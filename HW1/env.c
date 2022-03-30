/*
 *  This is a program that can print out the enviroment variables and
 *  execute commands that will remove and trace the command.
 *  Don't be confused like i was :')
 *  Apparently the question asks us to also just add the environment
 *  variables to the executeable independatly. Meaning the execvp is
 *  used for only adding the "name=value" pairs only. As for the optional
 *  paramenters and commands passed by the -v and -u flags, we count and remove
 *  them and execute them in the current file and not the new one we create from
 *  the execvp file.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

extern char **environ;

void print_env()
{
    while (*environ != NULL)
    {
        printf("%s\n", *environ);
        environ++;
    }
}

void remove_env(char *var)
{
    unsetenv(var);
    printf("\nRemoved!\n\n");
}

void add_env(char *var)
{
    putenv(var);
    printf("\nAdded!\n\n");
    
}

int is_env(char *var)
{
    char *temp = var;

    while (*temp != '\0')
    {

        if (*temp == '=')
            return 1;
        temp++;
    }
    return 0;
}

int main(int argc, char **argv)
{
    int c;
    int count = 0;
    
    // To print the already exiting env
    if (argc == 1)
    {
        print_env();
        return 0;
    }


    while ((c = getopt(argc, argv, "vu:")) != -1)
    {
        switch (c)
        {
        case 'u': // remove the optarg from env
            count += 2;
            remove_env(optarg);
            break;
        case 'v': // TODO Write the trace to standard error
            count++;
            break;
        default:
            fprintf(stderr, "Pass approprate flags. i.e -u [name] -v");
            break;
        }
    }

    char *arguments[argc - count];

    if(count == 0 && argc > 1){
        for (size_t i = 1; i < argc; i++)
        {
            if(is_env(argv[i])){
                add_env(argv[i++]);
            }
        }
        print_env();
    }

    execvp(argv[1], arguments);



    return 0;
}