#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>

#define CAPACITY_DEFAULT 8

/// @brief A growable vector.
struct vec
{
    void **ptr;
    size_t len;
    size_t capacity;
};

struct vec vec_new();

struct vec vec_new_with_capacity(size_t capacity);

// Get an element from the vector. This function performs bounds checking.
void *vec_get(struct vec *v, size_t index);

// Push an element to the end of the vector.
void vec_push(struct vec *v, void *elem);
// Pop an element from the end of the vector and return it.
void *vec_pop(struct vec *v);

// Insert an element into the vector. All elements at and proceeding the index are shifted forwards by one index to make space.
void vec_insert(struct vec *v, size_t index, void *elem);
// Remove an element from the vector and return it. All elements proceeding the removed element are shifted back by one index.
void *vec_remove(struct vec *v, size_t index);

// Destroys a vector. Does not free any elements.
void vec_destroy(struct vec *v);
// Destroys a vector and frees all elements.
void vec_destroy_deep(struct vec *v);