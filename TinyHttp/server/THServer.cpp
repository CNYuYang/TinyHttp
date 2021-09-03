#include "THServer.h"
#include "THGlobal.h"
#include <cstdio>
#include <mutex>
#include <netdb.h>
#include <openssl/ssl.h>

#define PORT_STR_MAX 5

int THServerBase::init(const struct sockaddr *bind_addr, socklen_t addrlen, const char *cert_file, const char *key_file) {

    /*超时时间选择，如果同时配置了peer_response_timeout/receive_timeout 选择其中较小的那个 */

    int timeout = this->params.peer_response_timeout;

    if (this->params.receive_timeout >= 0) {
        if ((unsigned int) timeout > (unsigned int) this->params.receive_timeout) {
            timeout = this->params.receive_timeout;
        }
    }

    //调用父类 @Link{Communicator.cpp} 的init()方法
    if (this->CommService::init(bind_addr, addrlen, -1, timeout) < 0)
        return -1;

    //设置ssl证书
    if (key_file && cert_file) {
        if (this->init_ssl_ctx(cert_file, key_file) < 0) {
            this->deinit();
            return -1;
        }
    }

    this->scheduler = THGlobal::get_scheduler();
    return 0;
}

/*暂时不配置ssl信息*/
// @TODO ssl相关
int THServerBase::init_ssl_ctx(const char *cert_file, const char *key_file) {
    return 0;
};


int THServerBase::start(int family, const char *host, unsigned short port,
                        const char *cert_file, const char *key_file) {
    struct addrinfo hints = {
            .ai_flags = AI_PASSIVE,
            .ai_family = family,
            .ai_socktype = SOCK_STREAM,
    };

    struct addrinfo *addrinfo;
    char port_str[PORT_STR_MAX + 1];
    int ret;
    snprintf(port_str, PORT_STR_MAX + 1, "%d", port);

    ret = getaddrinfo(host, port_str, &hints, &addrinfo);

    //获取成功
    if (ret == 0) {
        ret = start(addrinfo->ai_addr, (socklen_t) addrinfo->ai_addrlen,
                    cert_file, key_file);
        freeaddrinfo(addrinfo);
    }

    else {
        if (ret != EAI_SYSTEM)
            errno = EINVAL;
        ret = -1;
    }

    return ret;
}

int THServerBase::start(const struct sockaddr *bind_addr, socklen_t addrlen, const char *cert_file,
                        const char *key_file) {
    SSL_CTX *ssl_ctx;
    // @TODO ssl相关
    if (this->init(bind_addr, addrlen, cert_file, key_file) >= 0) {

        //绑定成功
        if (this->scheduler->bind(this) >= 0)
            return 0;

        //失败释放资源
        ssl_ctx = this->get_ssl_ctx();
        this->deinit();
        if (ssl_ctx)
            SSL_CTX_free(ssl_ctx);
    }

    return -1;
}

void THServerBase::shutdown() {
    this->scheduler->unbind(this);
}

void THServerBase::wait_finish() {
    SSL_CTX *ssl_ctx = this->get_ssl_ctx();
    std::unique_lock<std::mutex> lock(this->mutex);

}
