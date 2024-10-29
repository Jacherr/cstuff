#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

/// Default vector capacity.
#define _VEC_CAPACITY_DEFAULT 32
/// Capacity multiplier when a vector is grown.
#define _VEC_CAPACITY_MULTIPLIER 2

/// A vector is a growable collection of elements.
/// All elements must be the same width, i.e., the same size in bytes.
struct vector {
	uint32_t capacity;
	uint32_t len;
	size_t elem_size;
	char* data;
};

/// Create a new vector with a default (growable) capacity based on _VEC_CAPACITY_DEFAULT. If the vector cannot be created, NULL is returned.
/// To free this vector, call vec_destroy.
struct vector* vec_new(size_t elem_size);
/// Create a new vector with a specified (growable) capacity. If the vector cannot be created, NULL is returned.
/// To free this vector, call vec_destroy.
struct vector* vec_new_with_capacity(size_t elem_size, uint32_t capacity);

/// Create a clone of this vector, with the same elements and properties, or NULL if a clone could not be created.
struct vector* vec_clone(struct vector* v);

/// Return the memory address of a certain element in the vector WITHOUT COPYING, or NULL if out of range. If you need a copy, use vec_get_copied.
/// The pointer returned by this function is managed by the vector and must not be freed manually. 
/// You should not rely on the pointer returned by this method between pushes or inserts, because if the vector grows, this pointer is invalidated.
/// You are usually better off returning a copy of the element with vec_get_copied.
void* vec_get(struct vector* v, uint32_t element);
/// Return a copy of a certain element in the vector, or NULL if out of range.
void* vec_get_copied(struct vector* v, uint32_t element);

/// Overwrites an element with a new element by copying at an arbitrary index. Returns the old element, or NULL if it could not be copied.
void* vec_set(struct vector* v, void* data, uint32_t index);

/// Push a new element to the end of the vector. The element is copied. The element must have a width of elem_size.
void vec_push(struct vector* v, void* data);
/// Insert an element at an arbitrary position in the vector.
void vec_insert(struct vector* v, void* data, uint32_t index);
/// Insert an element at the start of the vector.
void vec_unshift(struct vector* v, void* data);

/// Remove the element from the end of the vector and return it as a copy. If the data cannot be copied, NULL is returned (but the element is still removed).
void* vec_pop(struct vector* v);
/// Remove an element from an arbitrary position in the vector and return it as a copy. If the data cannot be copied, NULL is returned (but the element is still removed).
void* vec_remove(struct vector* v, uint32_t index);
/// Remove the first element from the vector and return it as a copy. If the data cannot be copied, NULL is returned (but the element is still removed).
void* vec_shift(struct vector* v);

/// Swap the positions of two elements in this vector.
void vec_swap(struct vector* v, uint32_t index1, uint32_t index2);

/// Reverses the elements in the vector and returns the reversed vector as a new vector.
struct vector* vec_reverse(struct vector* v);
/// Reverses the elements in the vector in-place. May be slower than returning a new vector for large collections.
void vec_reverse_inplace(struct vector *v);

/// Increase the capacity of this vector based on the _VEC_CAPACITY_MULTIPLIER value.
void vec_grow(struct vector* v);

/// Get the amount of elements currently stored in this vector.
uint32_t vec_len(struct vector* v);

/// Search this vector for any elements that match, by value, the provided needle. The needle must have a width of elem_size.
int vec_contains(struct vector* v, void* needle);

/// Map each value in this vector to another and push it into a new vector. 
/// The mapper function must return a pointer to the new value which is copied into the new vector.
/// After insertion, `vec_map' frees the returned value from the mapper. 
/// Once all elements have been mapped, the new vector is returned.
/// The mapper may return NULL on error. This function will treat NULL as an error condition, end the map and return NULL.
struct vector* vec_map(struct vector* v, void* (*mapfn)(void*, uint32_t));

/// Works similarly to `vec_map', but only pushes elements to the new vector that have a return value greater than 0 from the filter function. All other values are discarded.
/// Implementation detail: while `vec_map' allows the new vector to inherit tht ecapacity of the old vector because the length is the same after mapping,
/// `vec_filter' does NOT inherit the old capacity because the length may be significantly less. This saves memory at the cost of more potential reallocations.
struct vector* vec_filter(struct vector* v, int (*filterfn)(void*, uint32_t));

/// Iterate over each value in this vector and perform an action. This function does not create a new vector.
/// The value pointer passed to `foreachfn' points to a *copy* of the value, so modifying this does not mutate the underlying vector.
/// `vec_foreach' handles the allocation and freeing of this pointer and should not be handled manually.
void vec_foreach(struct vector* v, void (*foreachfn)(void*, uint32_t));

/// Reset this vector, clearing all elements. Capacity and element width are not altered.
/// This method allows for the reuse of a vector without needing reallocation, saving time.
void vec_reset(struct vector* v);

/// Destroy this vector, freeing the internal buffer.
/// This should always be called after a vector is no longer useful.
void vec_destroy(struct vector* v);