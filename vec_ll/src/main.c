#include <stdio.h>
#include <stdint.h>

#include "vec/vector.h"

void* mapperfnmul2(void* elem, uint32_t index)
{
    int* db = malloc(sizeof(int));
    if (db == NULL)
    {
        return NULL;
    }
    *db = *(int*)elem * 2;
    return db;
}

int filterfn(void* elem, uint32_t index)
{
    if (*(int*)elem % 2 == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

void foreachfn(void* elem, uint32_t index)
{
    printf("foreach: element %i: %i\n", index, *(int*)elem);
}

int main() {
    struct vector* v = vec_new(sizeof(int));
    if (v == NULL)
    {
        return 1;
    }

    for (int i = 0; i < 10; i++)
    {
        vec_push(v, &i);
    }

    printf("cap: %i, len: %i\n", v->capacity, v->len);
    printf("5th element: %i\n", *(int*)vec_get(v, 5));

    int ins = 123;
    vec_insert(v, &ins, 5);
    printf("5th element now: %i\n", *(int*)vec_get(v, 5));
    printf("6th element now: %i\n", *(int*)vec_get(v, 6));

    int search = 100;
    printf("contains %i? %s\n", search, vec_contains(v, &search) ? "yes" : "no");

    int* removed = vec_remove(v, 5);
    printf("removed element: %i\n", *removed);
    printf("5th element now: %i\n", *(int*)vec_get(v, 5));
    printf("6th element now: %i\n", *(int*)vec_get(v, 6));

    struct vec* new = vec_map(v, mapperfnmul2);
    printf("5th element in new: %i\n", *(int*)vec_get(new, 5));
    printf("6th element in new: %i\n", *(int*)vec_get(new, 6));

    struct vector* filtered = vec_filter(v, filterfn);
    printf("2nd element in new filtered: %i\n", *(int*)vec_get(filtered, 2));
    printf("4th element in new filtered: %i\n", *(int*)vec_get(filtered, 4));
    printf("filtered: cap: %i, len: %i\n", filtered->capacity, filtered->len);

    int f_n = 10;
    vec_set(filtered, &f_n, 2);
    printf("2nd element in new filtered after setting: %i\n", *(int*)vec_get(filtered, 2));
    vec_foreach(filtered, foreachfn);

    printf("reversed:\n");
    struct vector* rev = vec_reverse(v);
    vec_foreach(rev, foreachfn);

    printf("reversed AGAIN:\n");
    vec_reverse_inplace(rev);
    vec_foreach(rev, foreachfn);

    vec_destroy(v);
    vec_destroy(rev);
    vec_destroy(filtered);
    vec_destroy(new);
    free(removed);

    return 0;
}
