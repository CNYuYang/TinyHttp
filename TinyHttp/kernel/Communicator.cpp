#include "Communicator.h"
#include "mpoller.h"
#include "poller.h"
#include "thrdpool.h"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <stdlib.h>

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
}

int Communicator::nonblock_listen(CommService *service) {
    int sockfd = service->create_listen_fd();
    if (sockfd >= 0) {
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
    }

    return -1;
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

    //@TODO 相信了解queue、mpoller
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
