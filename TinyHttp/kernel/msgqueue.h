#ifndef TINYHTTP_MSGQUEUE_H
#define TINYHTTP_MSGQUEUE_H

#include <sys/types.h>

typedef struct __msgqueue msgqueue_t;

msgqueue_t *msgqueue_create(size_t maxlen, int linkoff);


#endif //TINYHTTP_MSGQUEUE_H
