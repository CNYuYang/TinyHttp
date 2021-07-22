#ifndef TINYHTTP_THSERVER_H
#define TINYHTTP_THSERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <atomic>
#include <functional>
#include "Communicator.h"
#include "CommScheduler.h"
#include "THTask.h"

struct THServerParams {
    int peer_response_timeout; /*每次读写操作的超时时间*/
    int receive_timeout; /*接收到整个信息的超时时间*/

};

static constexpr struct THServerParams SERVER_PARAMS_DEFAULT = {
        .peer_response_timeout = 10 * 1000,
        .receive_timeout = -1
};

class THServerBase : protected CommService {

public:
    THServerBase(const struct THServerParams *params) :
            conn_count(0) {
        this->params = *params;
        //this->listen_fd = -1;
        //this->unbind_finish = false;
    }

public:
    /* Start on port with IPv4. */
    int start(unsigned short port)
    {
        return start(AF_INET, NULL, port, NULL, NULL);
    }

    int start(int family, const char *host, unsigned short port,
              const char *cert_file, const char *key_file);

    /* 真正的启动函数 */
    int start(const struct sockaddr *bind_addr, socklen_t addrlen,
              const char *cert_file, const char *key_file);

public:
    size_t get_conn_count() const { return this->conn_count; }

protected:
    THServerParams params;

private:
    int init_ssl_ctx(const char *cert_file, const char *key_file);
    int init(const struct sockaddr *bind_addr, socklen_t addrlen, const char *cert_file, const char *key_file);
protected:
    std::atomic<size_t> conn_count;
private:
    CommScheduler *scheduler;
};

//@TODO 未开始
template<class REQ, class RESP>
class THServer : public THServerBase {
public:
    THServer(std::function<void(THNetworkTask<REQ, RESP> *)> proc) :
            THServerBase(&SERVER_PARAMS_DEFAULT),
            process(std::move(proc)) {
    }

protected:
    std::function<void(THNetworkTask<REQ, RESP> *)> process;
};


#endif //TINYHTTP_THSERVER_H
