#ifndef TINYHTTP_COMMSCHEDULER_H
#define TINYHTTP_COMMSCHEDULER_H

#include "Communicator.h"

class CommScheduler {

public:
    int bind(CommService *a);

private:
    Communicator comm;
};


#endif //TINYHTTP_COMMSCHEDULER_H
