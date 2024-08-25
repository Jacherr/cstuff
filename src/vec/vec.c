#include <stdio.h>

#include "vec.h"

struct vec vec_new()
{
    struct vec v =
        {
            .ptr = NULL,
            .len = 0,
            .capacity = CAPACITY_DEFAULT,
        };

    return v;
}

struct vec vec_new_with_capacity(size_t capacity)
{
    struct vec v =
        {
            .ptr = NULL,
            .len = 0,
            .capacity = capacity,
        };

    return v;
}

void *vec_get(struct vec *v, size_t index)
{
    if (index > v->len)
    {
        return NULL;
    }
    return v->ptr[index];
}

void vec_push(struct vec *v, void *elem)
{
    if (v->ptr == NULL)
    {
        v->ptr = malloc(sizeof(elem) * v->capacity);
        v->ptr[v->len] = elem;
        v->len++;
    }
    else if (v->len == v->capacity)
    {
        v->capacity *= 2;
        v->ptr = realloc(v->ptr, sizeof(elem) * v->capacity);
        v->ptr[v->len] = elem;
        v->len++;
    }
    else
    {
        v->ptr[v->len] = elem;
        v->len++;
    }
}

void *vec_pop(struct vec *v)
{
    void *elem = v->ptr[v->len - 1];
    v->ptr[v->len - 1] = NULL;
    v->len--;
    return elem;
}

void vec_insert(struct vec *v, size_t index, void *elem)
{
    // inserting an element may cause the vec to exceed its capacity
    // but we do not want to re-implement reallocation code, so we can
    // pop the final element first, then insert our new element, then push
    // the final element again. this way, the capacity cannot be exceeded
    // until the push (and pushing elements can cause a reallocation)
    void *end = vec_pop(v);

    // move all elements at `index` and proceeding it to make space
    // v->len is decremented on pop so this should not overflow
    void *current = v->ptr[v->len];
    for (int i = v->len + 1; i > index; i--)
    {
        v->ptr[i] = current;
        current = v->ptr[i - 2];
    }

    // insert new element
    v->ptr[index] = elem;

    // we need to increase v->len by 1 since we inserted new element
    // this will allow the push to be handled properly
    v->len++;

    // push final element once again
    vec_push(v, end);
}

void *vec_remove(struct vec *v, size_t index)
{
    if (index > v->len || v->ptr == NULL)
    {
        return NULL;
    }

    void *removed;

    for (int i = index; i < v->len; i++)
    {
        if (i == index)
        {
            removed = v->ptr[i];
        }
        v->ptr[i] = v->ptr[i + 1];
    }

    v->len--;
    return removed;
}

void vec_destroy(struct vec *v)
{
    free(v->ptr);
}

void vec_destroy_deep(struct vec *v)
{
    for (int i = 0; i < v->len; i++)
    {
        free(v->ptr[i]);
    }

    vec_destroy(v);
}