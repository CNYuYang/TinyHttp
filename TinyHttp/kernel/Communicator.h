#ifndef TINYHTTP_COMMUNICATOR_H
#define TINYHTTP_COMMUNICATOR_H

#include <pthread.h>
#include <sys/socket.h>
#include "msgqueue.h"

class CommService {

public:
    int init(const struct sockaddr *bind_addr, socklen_t addrlen,
             int listen_timeout, int response_timeout);

    void deinit();

    int drain(int max);

private:
    struct sockaddr *bind_addr;
    socklen_t addrlen;
    int listen_timeout;
    int response_timeout;

private:
    pthread_mutex_t mutex;

};

class Communicator {
public:
    int init(size_t poller_threads, size_t handler_threads); // poller_threads个线程用于轮询 handler_threads个线程用于创建任务

private:
    struct __mpoller *mpoller;
    struct __msgqueue *queue;
    struct __thrdpool *thrdpool;
    int stop_flag;
private:
    int create_poller(size_t poller_threads);
    int create_handler_threads(size_t handler_threads);

    static void handler_thread_routine(void *context);

};

#endif //TINYHTTP_COMMUNICATOR_H
