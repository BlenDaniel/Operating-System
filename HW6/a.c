/* a.c */ 


#include <stdio.h> 

extern int x;
int y;


static void f()
{
    static char z = 'Z';
    puts("a.c: f()");
}

void g()
{
    puts("a.c: g()");
    f();
}