#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/select.h>
#include <fcntl.h>
#include "tcp.h"

extern int run(int fd);

void *thread_run(void *data)
{
    if (run(*((int *)data)) < 0)
    {
        perror("Thread run");
        return (void *)-1;
    }
    tcp_close(*((int *)data));
    return (void *)0;
}

int init_method(int fd4, int fd6, int choice)
{
    fprintf(stdout, "init_method\n");
    int cfd;

    fd_set fdset;

    while (1)
    {
        FD_ZERO(&fdset);
        FD_SET(fd4, &fdset);
        FD_SET(fd6, &fdset);

        if (select(fd4 > fd6 ? fd4 + 1 : fd6 + 1, &fdset, NULL, NULL, NULL) == -1)
        {
            perror("select");
            return -1;
        }

        switch (choice)
        {
        case 0: /* blocking method */

            if (FD_ISSET(fd4, &fdset))
            {

                cfd = tcp_accept(fd4);
                if (cfd == -1)
                {
                    perror("Accept");
                    return -1;
                }
                fprintf(stdout, "Block: Found a player: fd4 %d\n", cfd);
                run(cfd);
                tcp_close(cfd);
            }

            if (FD_ISSET(fd6, &fdset))
            {

                cfd = tcp_accept(fd6);
                if (cfd == -1)
                {
                    perror("Accept");
                    return -1;
                }
                fprintf(stdout, "Block: Found a player: fd6 %d\n", cfd);
                run(cfd);
                tcp_close(cfd);
            }

            break;
        case 1: /* thread method */

            if (FD_ISSET(fd4, &fdset))
            {
                pthread_t thread_id;

                /* 
                * ሌላ ጊዜ ሊያስፈልገኝ ይችላል።
                *
                int *arg = (int *)malloc(sizeof(intxx));
                if (arg == NULL)
                {
                    
                    exit(EXIT_FAILURE);
                }
                *arg = cfd;
                */

                int *p = malloc(sizeof(int));
                if (!p)
                {
                    fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
                }
                *p = tcp_accept(fd4);
                if (*p == -1)
                {

                    perror("Accept");
                    free(p);
                    return -1;
                }

                fprintf(stdout, "Thread: Found a player: fd4 %d\n", cfd);

                if (pthread_create(&thread_id, NULL, thread_run, p) < 0)
                {
                    perror("could not create thread");
                    return 1;
                }

                //Now join the thread , so that we dont terminate before the thread
                //pthread_join(thread_id, NULL);
            }

            if (FD_ISSET(fd6, &fdset))
            {
                pthread_t thread_id;

                int *p = malloc(sizeof(int));
                if (!p)
                {
                    fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
                }
                *p = tcp_accept(fd6);
                if (*p == -1)
                {

                    perror("Accept");
                    free(p);
                    return -1;
                }

                fprintf(stdout, "Thread: Found a player: fd6 %d\n", cfd);

                if (pthread_create(&thread_id, NULL, thread_run, p) < 0)
                {
                    perror("could not create thread");
                    return 1;
                }
                //Now join the thread , so that we dont terminate before the thread
                //pthread_join(thread_id, NULL);
            }

            break;
        case 2: /* fork method */

            FD_ZERO(&fdset);
            FD_SET(fd4, &fdset);
            FD_SET(fd6, &fdset);

            if (select(fd4 > fd6 ? fd4 + 1 : fd6 + 1, &fdset, NULL, NULL, NULL) == -1)
            {
                perror("select");
                return -1;
            }

            if (FD_ISSET(fd4, &fdset))
            {
                pid_t pid;
                if (fork() == 0)
                {
                    pid = getpid();

                    cfd = tcp_accept(fd4);
                    if (cfd == -1)
                    {
                        perror("Accept");
                        return -1;
                    }

                    fprintf(stdout, "Fork: Found a player and created a child process with pid: %d, fd4 : %d\n", pid, cfd);
                    run(cfd);
                    tcp_close(cfd);
                    exit(0);
                }
            }

            if (FD_ISSET(fd6, &fdset))
            {
                pid_t pid;
                if (fork() == 0)
                {
                    pid = getpid();

                    cfd = tcp_accept(fd6);
                    if (cfd == -1)
                    {
                        perror("Accept");
                        return -1;
                    }

                    fprintf(stdout, "Fork: Found a player and created a child process with pid: %d, fd6 : %d\n", pid, cfd);
                    run(cfd);
                    tcp_close(cfd);
                    exit(0);
                }
            }
            break;
        default:
            break;
        }
    }

    return 0;
}
