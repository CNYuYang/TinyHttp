//
// Created by y00612228 on 2021/7/20.
//

#ifndef TINYHTTP_COMMUNICATOR_H
#define TINYHTTP_COMMUNICATOR_H

#include <pthread.h>


class CommService{

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


#endif //TINYHTTP_COMMUNICATOR_H
