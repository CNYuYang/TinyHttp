//
// Created by 余阳 on 2021/7/21.
//
#include "THGlobal.h"
#include <signal.h>

class __THGlobal {

public:
    static __THGlobal *get_instance() {
        static __THGlobal kInstance;
        return &kInstance;
    }

    const THGlobalSettings *get_global_settings() const {
        return &settings_;
    }

private:
    __THGlobal();

private:
    struct THGlobalSettings settings_;
};

__THGlobal::__THGlobal() : settings_(GLOBAL_SETTINGS_DEFAULT) {
}

class __CommManager {
public:
    static __CommManager *get_instance() {
        static __CommManager kInstance;
        return &kInstance;
    }

    CommScheduler *get_scheduler() { return &scheduler_; }

private:
    __CommManager() {
        //if(scheduler_.init())
        const auto *settings = __THGlobal::get_instance()->get_global_settings();
        if (scheduler_.init(settings->poller_threads,
                            settings->handler_threads) < 0)
            abort();
    }

private:
    CommScheduler scheduler_;
};

CommScheduler *THGlobal::get_scheduler() {

    return __CommManager::get_instance()->get_scheduler();
}
