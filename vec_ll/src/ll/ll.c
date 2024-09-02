#include "ll.h"

struct record_ll *record_ll_new()
{
    struct record_ll *ll = malloc(sizeof(struct record_ll));
    if (ll == NULL)
    {
        return NULL;
    }
    ll->head = NULL;
    ll->len = 0;
    return ll;
}

struct record *record_ll_get(struct record_ll *ll, size_t index)
{
    if (index >= ll->len)
    {
        return NULL;
    }

    struct record *current = ll->head;
    for (int i = 0; i < index; i++)
    {
        current = current->next;
    }

    return current;
}