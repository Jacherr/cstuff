#include <stddef.h>

struct record
{
    char *name;
    int age;
    struct record *next;
};

struct record_ll
{
    struct record *head;
    int len;
};

struct record_ll *record_ll_new();

struct record *record_ll_get(struct record *first, size_t index);