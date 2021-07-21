//
// Created by y00612228 on 2021/7/20.
//

#ifndef TINYHTTP_THSERVER_H
#define TINYHTTP_THSERVER_H

#include <sys/types.h>
#include <atomic>
#include "../kernel/Communicator.h"

struct THServerParams {
};

static constexpr struct THServerParams SERVER_PARAMS_DEFAULT = {
};


class THServerBase : protected CommService {

public:
    THServerBase(const struct THServerParams *params) :
            conn_count(0) {
        this->params = *params;
        //this->listen_fd = -1;
        // this->unbind_finish = false;
    }

public:
    /* 真正的启动函数 */
    int start(const struct sockaddr *bind_addr, socklen_t addrlen,
              const char *cert_file, const char *key_file);

public:
    size_t get_conn_count() const { return this->conn_count; }

protected:
    THServerParams params;

protected:
    std::atomic <size_t> conn_count;

};


#endif //TINYHTTP_THSERVER_H
