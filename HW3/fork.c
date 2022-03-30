#include <stdio.h>
#include <unistd.h>

static int x = 0;

int main(int argc, char *argv[])
{
    pid_t p = getpid();

    x++;
    fork();
    if (!fork())
    {
        if (fork())
        {
            x++;
        }
        x++;
    }

    printf("p%d: x = %d\n", getpid() - p, x);
    return 0;
}

/*
if (!fork()) // same as: if (fork() == 0)
{
  // child
}else{
  // parent
}
*/