#include <stdio.h>

#include "vec/vec.h"
#include "ll/ll.h"

int main(int argc, char *argv[])
{
    struct vec v = vec_new();
    for (int i = 0; i < 100; i++)
    {
        int *ip = malloc(sizeof(int));
        *ip = i;
        vec_push(&v, ip);
    }

    int *elem = vec_remove(&v, 50);
    vec_insert(&v, 25, elem);

    vec_destroy_deep(&v);
}