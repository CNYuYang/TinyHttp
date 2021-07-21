//
// Created by 余阳 on 2021/7/21.
//

#ifndef TINYHTTP_THGLOBAL_H
#define TINYHTTP_THGLOBAL_H

#include "CommScheduler.h"

class THGlobal {

public:
    // Internal usage only
    static CommScheduler *get_scheduler();

};


#endif //TINYHTTP_THGLOBAL_H
