#ifndef TINYHTTP_COMMUNICATOR_H
#define TINYHTTP_COMMUNICATOR_H

#include "msgqueue.h"
#include "poller.h"
#include <openssl/ssl.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>

class CommTarget {

public:
    int init(const struct sockaddr *addr, socklen_t addrlen,
             int connect_timeout, int response_timeout);
};

class CommMessageOut {

private:
    // 编码
    virtual int encode(struct iovec vectors[], int max) = 0;

public:
    virtual ~CommMessageOut() {}

    friend class Communicator;
};

class CommMessageIn : private poller_message_t {

protected:
    virtual int append(const void *buf, size_t *size) = 0;

protected:
    /* 接收时发送 small packet，仅在append()函数中调用 */
    virtual int feedback(const void *buf, size_t size);

    //@TODO
public:
    virtual ~CommMessageIn() {}

    friend class Communicator;
};


class CommService {

public:
    int init(const struct sockaddr *bind_addr, socklen_t addrlen,
             int listen_timeout, int response_timeout);

    void deinit();

    int drain(int max);

protected:
    SSL_CTX *get_ssl_ctx() const { return this->ssl_ctx; }

private:
    virtual int create_listen_fd() {
        return socket(this->bind_addr->sa_family, SOCK_STREAM, 0);
    }

private:
    struct sockaddr *bind_addr;
    socklen_t addrlen;
    int listen_timeout;
    int response_timeout;
    SSL_CTX *ssl_ctx;

public:
    void incref() {
        __sync_add_and_fetch(&this->ref, 1);
    }

private:
    int listen_fd;
    int ref;

private:
    pthread_mutex_t mutex;

public:
    friend class Communicator;
};

class Communicator {
public:
    int init(size_t poller_threads, size_t handler_threads);// poller_threads个线程用于轮询 handler_threads个线程用于创建任务
    void deinit();

    int bind(CommService *service);                         //绑定服务
    void unbind(CommService *service);

private:
    struct __mpoller *mpoller;
    struct __msgqueue *queue;
    struct __thrdpool *thrdpool;
    int stop_flag;

private:
    int create_poller(size_t poller_threads);

    int create_handler_threads(size_t handler_threads);

    static void handler_thread_routine(void *context);

    int nonblock_listen(CommService *service);

    static void *accept(const struct sockaddr *addr, socklen_t addrlen,
                        int sockfd, void *context);
};

#endif//TINYHTTP_COMMUNICATOR_H
