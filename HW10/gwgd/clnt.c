/*
 * HW10/gwgd/clnt.c --
 *
 *	Creation and deletion of clients, providing a broadcast API.
 */

#define _POSIX_C_SOURCE 201112L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <event2/event.h>

#include "tcp.h"
#include "clnt.h"


clnt_t *clnt_new()
{
    clnt_t *clnt;
    
    clnt = calloc(1, sizeof(clnt_t));
    if (!clnt)
    {
        perror("calloc");
        return NULL;
    }

    clnt->player = player_new();
    if (!clnt->player)
    {
        player_del(clnt->player);
        return NULL;
    }

    return clnt;
}

void clnt_del(clnt_t *me)
{
    event_del(me->event);
    (void)tcp_close(me->fd);
    player_del(me->player);

}
/*
 * ምንም ጥቅም የለውም, ግን ምናልባት አንድ ቀን ጥቅም ሊኖረው ይችላል :)
*/

/*

void clnt_scast(clnt_t *client, const char *format, ...)
{
    va_list ap;
    char buf[1024];
    int len, rc;
    clnt_t *clnt, *gone = NULL;
    va_start(ap, format);
    len = vsnprintf(buf, sizeof(buf), format, ap);
    if (len > 0)
    {
        clnt = client;
        tcp_write(clnt->fd, buf, len);
    }
    
}


void clnt_bcast(const char *format, ...)
{
    va_list ap;
    char buf[1024];
    int len, rc;
    clnt_t *clnt, *gone = NULL;

    va_start(ap, format);
    len = vsnprintf(buf, sizeof(buf), format, ap);
    if (len > 0)
    {
        for (clnt = clients; clnt; clnt = clnt->next)
        {
            rc = tcp_write(clnt->fd, buf, len);
            if (rc <= 0)
                gone = clnt;
        }
    }
    if (gone)
        clnt_del(gone);
}
*/
