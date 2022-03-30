#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

static int n = 128;

void print_args(char **args);
int is_num(char *var);
void free_array(char **argv, int argc);
void run_command(char **argv, int show_args);

int main(int argc, char **argv)
{

    int c;
    int show_args = 0;
    char **arg_list = NULL;

    while ((c = getopt(argc, argv, "tjn:")) != -1)
    {

        switch (c)
        {
        case 't':
            show_args = 1;
            break;
        case 'j':
            /* code */
            break;
        case 'n':
            if (is_num(optarg))
            {
                n = atoi(optarg);
                if (n > 0)
                    break;
            }
            fprintf(stderr, "Number of arguments must be a non-zero positive nunber\n");
            exit(EXIT_FAILURE);
            break;

        default:
            printf("Usage: xargs [-n <args>][-t] command arg ...\n");
            return EXIT_SUCCESS;
        }
    }

    argc -= optind;
    argv += optind;

    arg_list = (char **)malloc(sizeof(char *) * (n + argc + 1));

    if (arg_list == NULL)
    {
        fprintf(stderr, "Unable to allocate buffer - error arg_list");
        exit(1);
    }

    if (argc == 0)
    {
        arg_list[0] = "/bin/echo";
        argc++;
    }
    else
    {
        for (int i = 0; i < argc; i++)
        {
            arg_list[i] = argv[i];
        }
    }

    for (int i = argc; i < n; i++)
    {
        arg_list[i] = NULL;
    }

    char *line = NULL;
    size_t len = 0;
    int count = 0;

    line = (char *)malloc(sizeof(char *));

    if (line == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }

    while (1)
    {
        if (count < n)
        {
            int chars = 0;

            if ((chars = getline(&line, &len, stdin)) == -1)
            {
                break;
            }

            // add a \0 at the end of the first line
            if (line[chars - 1] == '\n')
            {
                line[chars - 1] = '\0';
            }

            //is used to duplicate a string.
            arg_list[argc + count] = strdup(line);
            count++;
        }

        if (count == n)
        {
            if (show_args == 1)
            {
                print_args(arg_list);
            }
            run_command(arg_list, show_args);

            free_array(arg_list, argc);
            count = 0;
        }
    }

    if (count != 0)
    {
        arg_list[argc + count] = NULL;

        if (show_args == 1)
        {
            print_args(arg_list);
        }

        run_command(arg_list, show_args);
        free_array(arg_list, argc);
    }

    free(line);
    free(arg_list);

    return EXIT_SUCCESS;
}

void run_command(char **argv, int show_args)
{
    int status;
    pid_t pid = fork();

    if (pid < 0)
    {
        fprintf(stderr, "xargs: fork: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        execvp(argv[0], argv);
        fprintf(stderr, "xargs: execvp: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (waitpid(pid, &status, 0) == -1)
    {
        fprintf(stderr, "xargs: waitpid: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (!WIFEXITED(status) || WEXITSTATUS(status))
    {
        /* idk if i need something here */
    }
}

void print_args(char **args)
{
    while (*args != NULL)
    {
        fprintf(stdout, "%s\n", *args);
        args++;
    }
}

int is_num(char *var)
{
    int checker = 1;
    while (*var)
    {
        if (!isdigit(*var))
        {
            checker = 0;
        }
        var++;
    }
    return checker;
}

void free_array(char **argv, int argc)
{
    for (int i = argc; i <= n; i++)
    {
        if (argv[i])
        {
            free(argv[i]);
        }
    }
}