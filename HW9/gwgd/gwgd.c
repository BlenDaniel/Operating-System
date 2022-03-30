/*
* gwgd.c --
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "tcp.h"

extern int init_method(int fd4, int fd6, int choice);

static void usage(FILE *stream, int status)
{
    (void)fprintf(stream, "usage: gwgd [-f] [-t] -p port\n");
    exit(status);
}

int main(int argc, char *argv[])
{

    int fd4, fd6, rc, c, m = 0;
    char *nPort;
    const char *interfaces[] = {"0.0.0.0", "::", NULL};

    while ((c = getopt(argc, argv, "tfp:")) != -1)
    {
        switch (c)
        {
        case 't':
            m = 1;
            break;
        case 'f':
            m = 2;
            break;
        case 'p':
            nPort = optarg;
            break;
        default:
            usage(stdout, EXIT_SUCCESS);
        }
    }

    fd4 = tcp_listen(interfaces[0], nPort);
    if (fd4 < 0)
    {
        perror("fd4");
        return EXIT_FAILURE;
    }
    
    fd6 = tcp_listen(interfaces[1], nPort);
    if (fd4 < 0)
    {
        perror("fd4");
        return EXIT_FAILURE;
    }

    rc = init_method(fd4, fd6, m);

    if (rc < -1)
    {
        fprintf(stderr, "Error listening\n");
        return EXIT_FAILURE;
    }

    if (!rc)
    {
        fprintf(stderr, "Error in method base creation\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
