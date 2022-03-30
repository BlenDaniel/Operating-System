/*
 * socket/chatd/clnt-event.c --
 *
 *	Client related event callbacks.
 */

#define _POSIX_C_SOURCE 201112L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <event2/event.h>

#include "tcp.h"
#include "clnt.h"

void clnt_read(evutil_socket_t evfd, short evwhat, void *evarg)
{

    char *msg;
    clnt_t *me = evarg;
    int rc;
    (void)evwhat;

    if (!(me->player->state & PLAYER_STATE_FINISHED))
    {
        char buf[1024];
        int len;

        len = tcp_read(evfd, buf, sizeof(buf));

        if (len <= 0)
        {
            perror("Read");
        }

        rc = player_post_challenge(me->player, buf, &msg);

        if (rc > 0)
        {
            (void)tcp_write(me->fd, msg, strlen(msg));
            (void)free(msg);
        }

        if (me->player->state & PLAYER_STATE_FINISHED)
        {
            clnt_del(me);
            return;
        }

        if (!(me->player->state & PLAYER_STATE_CONTINUE))
        {
            player_fetch_chlng(me->player);
        }

        rc = player_get_challenge(me->player, &msg);

        if (rc > 0)
        {
            (void)tcp_write(me->fd, msg, strlen(msg));
            (void)free(msg);
        }
    }
    else
    {
        clnt_del(me);
    }
}

void clnt_join(evutil_socket_t evfd, short evwhat, void *evarg)
{
    int cfd, rc;
    clnt_t *clnt;
    char *msg;

    struct event_base *evb = evarg;

    player_t *p;

    (void)evwhat;

    cfd = tcp_accept(evfd);

    if (cfd == -1)
    {
        return;
    }

    (void)fcntl(cfd, F_SETFL, O_NONBLOCK);

    clnt = clnt_new();
    p = player_new();

    if (!p)
    {
        return;
    }

    if (!clnt)
    {
        return;
    }
    clnt->fd = cfd;
    clnt->player = p;

    /*
     *  Not event but rather an initialization of an event.
     *  So, it can be called at the bottom of event or top. Doesn't matter. :)
     */

    rc = player_get_greeting(clnt->player, &msg);
    if (rc > 0)
    {
        (void)tcp_write(clnt->fd, msg, strlen(msg));
        (void)free(msg);
    }

    if (!(clnt->player->state & PLAYER_STATE_CONTINUE))
    {
        player_fetch_chlng(clnt->player);
    }
    rc = player_get_challenge(clnt->player, &msg);
    if (rc > 0)
    {
        (void)tcp_write(clnt->fd, msg, strlen(msg));
        (void)free(msg);
    }

    /*
     * This is where the event happens
     */

    clnt->event = event_new(evb, cfd, EV_READ | EV_PERSIST, clnt_read, clnt);
    (void)event_add(clnt->event, NULL);
}
