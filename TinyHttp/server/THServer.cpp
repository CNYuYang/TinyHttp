#ifndef TINYHTTP_THSERVER_H
#define TINYHTTP_THSERVER_H

#include "CommScheduler.h"
#include "Communicator.h"
#include "THTask.h"
#include <atomic>
#include <functional>
#include <mutex>
#include <sys/socket.h>
#include <sys/types.h>

struct THServerParams {
    int peer_response_timeout; /*每次读写操作的超时时间*/
    int receive_timeout;       /*接收到整个信息的超时时间*/
};

static constexpr struct THServerParams SERVER_PARAMS_DEFAULT = {
        .peer_response_timeout = 10 * 1000,
        .receive_timeout = -1};

class THServerBase : protected CommService {

public:
    THServerBase(const struct THServerParams *params) : conn_count(0) {
        this->params = *params;
        //this->listen_fd = -1;
        //this->unbind_finish = false;
    }

public:
    /* 监听 IPv4 的端口. */
    int start(unsigned short port) {
        return start(AF_INET, NULL, port, NULL, NULL);
    }

    /* 选择IP协议 IPV4:AF_INET IPV6:AF_INET6 */
    int start(int family, unsigned short port) {
        return start(family, NULL, port, NULL, NULL);
    }

    /* 用hostname和端口号开启一个IPV4服务 */
    int start(const char *host, unsigned short port) {
        return start(AF_INET, host, port, NULL, NULL);
    }

    /* 用IP协议、hostname和端口号开启一个IPV4服务*/
    int start(int family, const char *host, unsigned short port) {
        return start(family, host, port, NULL, NULL);
    }


    int start(int family, const char *host, unsigned short port,
              const char *cert_file, const char *key_file);

    /* 真正的启动函数 */
    int start(const struct sockaddr *bind_addr, socklen_t addrlen,
              const char *cert_file, const char *key_file);

    /* stop() 是一个阻塞操作 */
    void stop() {
        this->shutdown();
        this->wait_finish();
    };

    void shutdown();
    void wait_finish();

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
    std::mutex mutex;
    CommScheduler *scheduler;
};

//@TODO 未开始
template<class REQ, class RESP>
class THServer : public THServerBase {
public:
    THServer(const struct THServerParams *params,
             std::function<void(THNetworkTask<REQ, RESP> *)> proc) : THServerBase(params),
                                                                     process(std::move(proc)) {
    }

    THServer(std::function<void(THNetworkTask<REQ, RESP> *)> proc) : THServerBase(&SERVER_PARAMS_DEFAULT),
                                                                     process(std::move(proc)) {
    }

protected:
    std::function<void(THNetworkTask<REQ, RESP> *)> process;
};


#endif//TINYHTTP_THSERVER_H
