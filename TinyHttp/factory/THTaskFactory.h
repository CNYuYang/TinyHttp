#ifndef TINYHTTP_WFTASKFACTORY_H
#define TINYHTTP_WFTASKFACTORY_H

#include "THTask.h"
#include "HttpMessage.h"

using THHttpTask = THNetworkTask<protocol::HttpRequest,
        protocol::HttpResponse>;

#endif //TINYHTTP_WFTASKFACTORY_H
