#ifndef TINYHTTP_COMMSCHEDULER_H
#define TINYHTTP_COMMSCHEDULER_H

#include <sys/types.h>
#include "Communicator.h"

class CommScheduler {

public:
    int init(size_t poller_threads, size_t handler_threads) {
        return this->comm.init(poller_threads, handler_threads);
    }

public:
    int bind(CommService *service) {
        return this->comm.bind(service);
    };

    void unbind(CommService *service)
    {
        this->comm.unbind(service);
    }

private:
    Communicator comm;
};


#endif //TINYHTTP_COMMSCHEDULER_H
