#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

#include <stdio.h>

struct arena
{
    char **chunks;
    uint32_t chunk_capacity;
    uint32_t chunk_count;
    size_t chunk_size;
    size_t offset;
};

struct arena *arena_create(size_t chunk_size, uint32_t chunk_capacity);
struct arena *arena_create_default();

void *arena_alloc(struct arena *arena, size_t size);
void *arena_alloc_align(struct arena *arena, size_t size, size_t align);

void arena_destroy(struct arena *arena);

void _arena_chunk_capacity_extend(struct arena *a);
void _arena_offset_align(struct arena *a, size_t align);