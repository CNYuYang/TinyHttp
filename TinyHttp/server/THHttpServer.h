#ifndef TINYHTTP_THHTTPSERVER_H
#define TINYHTTP_THHTTPSERVER_H

#include "THServer.h"
#include "HttpMessage.h"

using http_process_t = std::function<void(WFHttpTask * )>;
using WFHttpServer = WFServer<
        protocol::HttpRequest,
        protocol::HttpResponse>;

#endif //TINYHTTP_THHTTPSERVER_H
