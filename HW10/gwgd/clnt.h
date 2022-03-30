/*
 * socket/clnt.h --
 *
 */

#ifndef _CLNT_H
#define _CLNT_H

#include <event2/event.h>
#include "player.h"

typedef struct clnt {
    evutil_socket_t   fd;
    struct event      *event;
    player_t *player;
} clnt_t;

clnt_t* clnt_new();
void clnt_del(clnt_t *clnt);

void clnt_join(evutil_socket_t evfd, short evwhat, void *evarg);
void clnt_read(evutil_socket_t evfd, short evwhat, void *evarg);

#endif
