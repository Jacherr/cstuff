#include "arena.h"

struct arena *arena_create(size_t chunk_size, uint32_t chunk_capacity)
{
    /* such a tiny chunk size is useful to nobody and complicates allocation a lot */
    if (chunk_size < alignof(max_align_t))
    {
        return NULL;
    }

    struct arena *a = malloc(sizeof(struct arena));
    a->chunk_size = chunk_size;
    a->chunk_count = 1;
    a->chunk_capacity = chunk_capacity;
    a->offset = 0;
    a->chunks = malloc(sizeof(char *) * a->chunk_capacity);
    a->chunks[0] = malloc(sizeof(char) * a->chunk_size);
    return a;
}

struct arena *arena_create_default()
{
    return arena_create(1024, 4);
}

void *arena_alloc_align(struct arena *a, size_t size, size_t align)
{
    uint32_t current_chunk = a->chunk_count - 1;
    /* align with data type */
    _arena_offset_align(a, align);
    /* if allocation overflows to next chunk */
    if ((a->offset + size) > a->chunk_size)
    {
        current_chunk += 1;

        /* if we have ran out of space for new chunks */
        if (current_chunk >= a->chunk_capacity)
        {
            /* extend chunk capacity to make space */
            _arena_chunk_capacity_extend(a);
        }

        a->chunks[current_chunk] = malloc(sizeof(char) * a->chunk_size);
        a->offset = 0;
        a->chunk_count += 1;
    }

    char *chunk_offset = a->chunks[current_chunk] + a->offset;
    a->offset += size;
    /* we have correct offset for next object: return it as ptr */
    return chunk_offset;
}

void *arena_alloc(struct arena *a, size_t size)
{
    return arena_alloc_align(a, size, alignof(max_align_t));
}

void arena_destroy(struct arena *a)
{
    for (int i = 0; i < a->chunk_count; i++)
    {
        free(a->chunks[i]);
    }
    free(a->chunks);
    free(a);
}

void _arena_offset_align(struct arena *a, size_t align)
{
    a->offset += (align - 1) & ~(align + 1);
}

void _arena_chunk_capacity_extend(struct arena *a)
{
    uint32_t new_capacity = a->chunk_capacity * 2;
    a->chunks = realloc(a->chunks, sizeof(char *) * new_capacity);
    a->chunk_capacity = new_capacity;
}