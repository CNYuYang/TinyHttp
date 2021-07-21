#include "THServer.h"

int
THServerBase::start(const struct sockaddr *bind_addr, socklen_t addrlen, const char *cert_file, const char *key_file) {

    /*超时时间选择，如果同时配置了peer_response_timeout/receive_timeout 选择其中较小的那个 */

    int timeout = this->params.peer_response_timeout;

    if (this->params.receive_timeout >= 0) {
        if ((unsigned int) timeout > (unsigned int) this->params.receive_timeout) {
            timeout = this->params.receive_timeout;
        }
    }

    //调用父类 @Link{Communicator} 的init()方法
    if (this->init(bind_addr, addrlen, -1, timeout) < 0)
        return -1;

    //设置ssl证书
    if (key_file && cert_file) {
        if (this->init_ssl_ctx(cert_file, key_file) < 0) {
            this->deinit();
            return -1;
        }
    }

    return 0;
}

/*暂时不配置ssl信息*/
// @TODO ssl相关
int THServerBase::init_ssl_ctx(const char *cert_file, const char *key_file) {
    return 0;
};