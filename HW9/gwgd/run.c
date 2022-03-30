/*
* block-method.c --
*/

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <fcntl.h>
#include "player.h"

#include "tcp.h"

int run(int fd)
{

    
    player_t *p;
    char *msg;
    int rc;

    p = player_new();

    if (!p)
    {
        return EXIT_FAILURE;
    }
    rc = player_get_greeting(p, &msg);

    if (rc > 0)
    {
        (void)tcp_write(fd, msg, strlen(msg));
        (void)free(msg);
    }

    while (!(p->state & PLAYER_STATE_FINISHED))
    {
        char buf[1024];
        int len;

        if (!(p->state & PLAYER_STATE_CONTINUE))
        {
            player_fetch_chlng(p);
        }
        rc = player_get_challenge(p, &msg);
        if (rc > 0)
        {
            (void)tcp_write(fd, msg, strlen(msg));
            (void)free(msg);
        }

        len = tcp_read(fd, buf, sizeof(buf));

        if (len <= 0)
        {
            perror("Read");
            return len;
        }

        rc = player_post_challenge(p, buf, &msg);
        
        if (rc > 0)
        {
            (void)tcp_write(fd, msg, strlen(msg));
            (void)free(msg);
        }
    }

    player_del(p);

    return EXIT_SUCCESS;
}
