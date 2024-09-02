#include <stdio.h>

#include "arena.h"

int main()
{
    for (int i = 0; i < 100000000; i++)
    {
        /* init */
        struct arena *a = arena_create(1024, 1);
        /* first alloc */
        char *alloc = arena_alloc(a, 64);
        /* fits in same chunk */
        char *alloc2 = arena_alloc(a, 32);
        /* also fits */
        char *alloc3 = arena_alloc(a, 100);

        arena_destroy(a);
    }
}