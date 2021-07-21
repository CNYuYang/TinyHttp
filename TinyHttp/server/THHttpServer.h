#ifndef TINYHTTP_THHTTPSERVER_H
#define TINYHTTP_THHTTPSERVER_H

#include <functional>
#include "THServer.h"
#include "HttpMessage.h"
#include "THTaskFactory.h"

using http_process_t = std::function<void(THHttpTask *)>;
using THHttpServer = THServer<
        protocol::HttpRequest,
        protocol::HttpResponse>;

static constexpr struct THServerParams HTTP_SERVER_PARAMS_DEFAULT =
        {
                .peer_response_timeout    =    10 * 1000,
                .receive_timeout        =    -1,
        };

template<>
inline THHttpServer::THServer(http_process_t
proc) :

THServerBase (&HTTP_SERVER_PARAMS_DEFAULT),
process(std::move(proc)) {

}

#endif //TINYHTTP_THHTTPSERVER_H
