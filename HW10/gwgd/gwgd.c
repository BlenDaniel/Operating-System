/*
* gwgd.c --
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "tcp.h"
#include "clnt.h"



static void usage(FILE *stream, int status)
{
    (void)fprintf(stream, "usage: gwgd [-p port]\n");
    exit(status);
}

int main(int argc, char *argv[])
{

    int fd, i, c;
    struct event_base *evb;
    struct event *ev;
    const char *interfaces[] = {"0.0.0.0", "::", NULL};

    char *port;

    while ((c = getopt(argc, argv, "p:")) != -1)
    {
        switch (c)
        {
        case 'p':
            port = optarg;
            break;
        default:
            usage(stdout, EXIT_SUCCESS);
        }
    }

    if(argc != 3){
        usage(stdout, EXIT_SUCCESS);
    }

    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        perror("signal");
        return EXIT_FAILURE;
    }

    evb = event_base_new();
    
    if (!evb)
    {
        fprintf(stderr, "event_base_new: failed\n");
        return EXIT_FAILURE;
    }
    for (i = 0; interfaces[i]; i++)
    {
        fd = tcp_listen(interfaces[i], port);
        if (fd == -1)
        {
            continue;
        }
        ev = event_new(evb, fd, EV_READ | EV_PERSIST, clnt_join, evb);
        event_add(ev, NULL);
    }
    
    if (event_base_loop(evb, 0) == -1)
    {
        fprintf(stderr, "event_base_loop: failed\n");
        event_base_free(evb);
        return EXIT_FAILURE;
    }
    
    (void)event_base_free(evb);
    (void)tcp_close(fd);

    return EXIT_SUCCESS;
}
