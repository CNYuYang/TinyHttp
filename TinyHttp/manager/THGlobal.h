//
// Created by 余阳 on 2021/7/21.
//

#ifndef TINYHTTP_THGLOBAL_H
#define TINYHTTP_THGLOBAL_H

#include "CommScheduler.h"


struct THGlobalSettings {
    int poller_threads;
    int handler_threads;
};

static constexpr struct THGlobalSettings GLOBAL_SETTINGS_DEFAULT = {
        .poller_threads		=	4,
        .handler_threads	=	20
};

class THGlobal {

public:
    // Internal usage only
    static CommScheduler *get_scheduler();

};


#endif //TINYHTTP_THGLOBAL_H
