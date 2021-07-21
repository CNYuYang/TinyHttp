#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "Communicator.h"


int CommService::init(const struct sockaddr *bind_addr, socklen_t addrlen,
                      int listen_timeout, int response_timeout) {

    int ret;

    this->bind_addr = (struct sockaddr *) malloc(addrlen);

    if (this->bind_addr) {
        ret = pthread_mutex_init(&this->mutex, NULL);
        if (ret == 0) {
            memcpy(this->bind_addr, bind_addr, addrlen);
            this->addrlen = addrlen;
            this->listen_timeout = listen_timeout;
            this->response_timeout = response_timeout;
            //INIT_LIST_HEAD(&this->alive_list);

            //this->ssl_ctx = NULL;
            //this->ssl_accept_timeout = 0;
            return 0;
        }
        errno = ret;
        free(this->bind_addr);
    }

    return -1;
}

void CommService::deinit() {
    pthread_mutex_destroy(&this->mutex);
    free(this->bind_addr);
}

int CommService::drain(int max) {
    return 1;
}