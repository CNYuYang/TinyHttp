//
// Created by 余阳 on 2021/7/21.
//

#include "THGlobal.h"


class __CommManager {
public:
    static __CommManager *get_instance() {
        static __CommManager kInstance;
        return &kInstance;
    }

    CommScheduler *get_scheduler() { return &scheduler_; }

private:
    __CommManager(){
        //if(scheduler_.init())
    }

private:
    CommScheduler scheduler_;
};

CommScheduler *THGlobal::get_scheduler() {

    return __CommManager::get_instance()->get_scheduler();

}
