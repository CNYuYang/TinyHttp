//
// Created by y00612228 on 2021/7/20.
//

#include "THServer.h"

int THServerBase::start(const struct sockaddr *bind_addr, int addrlen, const char *cert_file, const char *key_file) {

    SSL *ssl_ctx;

    if(this -> init())

}