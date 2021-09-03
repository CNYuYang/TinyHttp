#include "Communicator.h"
#include "mpoller.h"
#include "thrdpool.h"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

static inline int __set_fd_nonblock(int fd) {
    int flags = fcntl(fd, F_GETFL);

    if (flags >= 0)
        flags = fcntl(fd, F_SETFL, flags | O_NONBLOCK);

    return flags;
}

static int __bind_and_listen(int sockfd, const struct sockaddr *addr,
                             socklen_t addrlen) {
    struct sockaddr_storage ss;
    socklen_t len;

    len = sizeof(struct sockaddr_storage);
    if (getsockname(sockfd, (struct sockaddr *) &ss, &len) < 0)
        return -1;

    ss.ss_family = 0;
    while (len != 0) {
        if (((char *) &ss)[--len] != 0)
            break;
    }

    if (len == 0) {
        if (bind(sockfd, addr, addrlen) < 0)
            return -1;
    }

    return listen(sockfd, SOMAXCONN);
}

int CommService::init(const struct sockaddr *bind_addr, socklen_t addrlen,
                      int listen_timeout, int response_timeout) {

    int ret;

    this->bind_addr = (struct sockaddr *) malloc(addrlen);

    if (this->bind_addr) {
        ret = pthread_mutex_init(&this->mutex, NULL);
        if (ret == 0) {
            memcpy(this->bind_addr, bind_addr, addrlen);
            this->addrlen = addrlen;
            this->listen_timeout = listen_timeout;
            this->response_timeout = response_timeout;
            //INIT_LIST_HEAD(&this->alive_list);

            //this->ssl_ctx = NULL;
            //this->ssl_accept_timeout = 0;
            return 0;
        }
        errno = ret;
        free(this->bind_addr);
    }

    return -1;
}

void CommService::deinit() {
    pthread_mutex_destroy(&this->mutex);
    free(this->bind_addr);
}

int CommService::drain(int max) {
    return 1;
}

int Communicator::bind(CommService *service) {
    struct poller_data data;
    int sockfd;
    sockfd = this->nonblock_listen(service);

    if (sockfd >= 0) {
        service->listen_fd = sockfd;
        service->ref = 1;
        data.operation = PD_OP_LISTEN;
        data.fd = sockfd;
        data.accept = Communicator::accept;
        data.context = service;
        data.result = NULL;
        if (mpoller_add(&data, service->listen_timeout, this->mpoller) >= 0) {
            return 0;
        }
        close(sockfd);
    }

    return -1;
}


void Communicator::unbind(CommService *service) {
}

//@TODO
int CommTarget::init(const struct sockaddr *addr, socklen_t addrlen,
                     int connect_timeout, int response_timeout) {
    return 0;
}

class CommServiceTarget : public CommTarget {
private:
    int sockfd;
    int ref;

private:
    CommService *service;

private:
    friend class Communicator;
};

//@TODO Target的作用
void *Communicator::accept(const struct sockaddr *addr, socklen_t addrlen,
                           int sockfd, void *context) {
    CommService *service = (CommService *) context;
    CommServiceTarget *target = new CommServiceTarget;

    if (target) {
        if (target->init(addr, addrlen, 0, service->response_timeout) >= 0) {
            service->incref();
            target->service = service;
            target->sockfd = sockfd;
            target->ref = 1;
            return target;
        }

        delete target;
    }

    close(sockfd);
    return nullptr;
}

int Communicator::nonblock_listen(CommService *service) {
    int sockfd = service->create_listen_fd();

    if (sockfd >= 0) {
        if (__set_fd_nonblock(sockfd) >= 0) {

            if (__bind_and_listen(sockfd, service->bind_addr, service->addrlen) >= 0) {
                return sockfd;
            }
        }
        close(sockfd);
    }

    return -1;
}

int Communicator::init(size_t poller_threads, size_t handler_threads) {

    //用于轮询的线程
    if (poller_threads == 0) {
        errno = EINVAL;
        return -1;
    }

    //创建用于轮询的线程
    if (this->create_poller(poller_threads) >= 0) {

        if (this->create_handler_threads(handler_threads) >= 0) {
            this->stop_flag = 0;
            return 0;
        }
        mpoller_stop(this->mpoller);
        mpoller_destroy(this->mpoller);
        msgqueue_destroy(this->queue);
    }

    return -1;
}

void Communicator::deinit() {
    this->stop_flag = 1;
    mpoller_stop(this->mpoller);
    msgqueue_set_nonblock(this->queue);
    thrdpool_destroy(nullptr, this->thrdpool);
    mpoller_destroy(this->mpoller);
    msgqueue_destroy(this->queue);
}

//创建用于轮询的线程
int Communicator::create_poller(size_t poller_threads) {

    //@TODO参数不齐
    struct poller_params params = {
            .max_open_files = 65536,
            .create_message = NULL,
            .partial_written = NULL,
            .callback = NULL,
            .context = this};

    //@TODO 详细了解queue、mpoller
    // 创建消息队列
    this->queue = msgqueue_create(4096, sizeof(struct poller_result));

    if (this->queue) {

        this->mpoller = mpoller_create(&params, poller_threads);

        if (this->mpoller) {
            if (mpoller_start(this->mpoller) >= 0) {
                return 0;
            }

            mpoller_destroy(this->mpoller);
        }
        msgqueue_destroy(this->queue);
    }

    return -1;
}

int Communicator::create_handler_threads(size_t handler_threads) {

    struct thrdpool_task task = {
            .routine = Communicator::handler_thread_routine,
            .context = this};
    size_t i;

    this->thrdpool = thrdpool_create(handler_threads, 0);

    if (this->thrdpool) {

        for (i = 0; i < handler_threads; ++i) {
            if (thrdpool_schedule(&task, this->thrdpool) < 0) break;
        }

        //创建成功
        if (i == handler_threads)
            return 0;

        //@TODO 这句代码的含义
        msgqueue_set_nonblock(this->queue);
        thrdpool_destroy(NULL, this->thrdpool);
    }

    return -1;
}

void Communicator::handler_thread_routine(void *context) {

    Communicator *comm = (Communicator *) context;
    struct poller_result *res;

    while ((res = (struct poller_result *) msgqueue_get(comm->queue)) != NULL) {
        while (res->data.operation) {
            //@TODO
        }

        //消息消费完，就删除
        free(res);
    }
}
