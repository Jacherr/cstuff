#include <stdio.h>

#include "vector.h"

struct vector* vec_new(size_t elem_size)
{
	return vec_new_with_capacity(elem_size, _VEC_CAPACITY_DEFAULT);
}

struct vector* vec_new_with_capacity(size_t elem_size, uint32_t capacity)
{
	struct vector* v = malloc(sizeof(struct vector));
	if (v == NULL)
	{
		errno = ENOMEM;
		return NULL;
	}

	v->capacity = capacity;
	v->len = 0;
	v->elem_size = elem_size;
	char* data = malloc(v->capacity * v->elem_size);
	if (data == NULL)
	{
		errno = ENOMEM;
		free(v);
		return NULL;
	}

	v->data = data;
	return v;
}

struct vector* vec_clone(struct vector* v)
{
	if (v == NULL)
	{
		errno = EINVAL;
		return NULL;
	}

	struct vector* new = malloc(sizeof(struct vector));
	if (new == NULL)
	{
		errno = ENOMEM;
		return NULL;
	}

	new->capacity = v->capacity;
	new->len = v->len;
	new->elem_size = new->elem_size;
	// create new pointer for data
	new->data = malloc(v->capacity * v->elem_size);
	if (new->data == NULL)
	{
		errno = ENOMEM;
		free(new);
		return NULL;
	}

	memcpy(new->data, v->data, v->len * v->elem_size);

	return new;
}

void* vec_get(struct vector* v, uint32_t element)
{
	if (v == NULL)
	{
		errno = EINVAL;
		return NULL;
	}

	if (element >= v->len)
	{
		errno = EINVAL;
		return NULL;
	}
	else
	{
		size_t offset = v->elem_size * element;
		return (void*)(v->data + offset);
	}
}

void* vec_get_copied(struct vector* v, uint32_t element)
{
	if (v == NULL)
	{
		errno = EINVAL;
		return NULL;
	}

	if (element >= v->len)
	{
		errno = EINVAL;
		return NULL;
	}
	else
	{
		size_t offset = v->elem_size * element;
		void* out = malloc(v->elem_size);
		if (out == NULL)
		{
			errno = ENOMEM;
			return NULL;
		}
		memcpy(out, v->data + offset, v->elem_size);
		return out;
	}
}

void* vec_set(struct vector* v, void* data, uint32_t index)
{
	if (v == NULL || index >= v->len)
	{
		errno = EINVAL;
		return NULL;
	}

	void* old = vec_get_copied(v, index);
	void* addr = vec_get(v, index);
	memcpy(addr, data, v->elem_size);

	return old;
}

void vec_push(struct vector* v, void* data)
{
	if (v == NULL)
	{
		errno = EINVAL;
		return;
	}

	if (v->len >= v->capacity)
	{
		vec_grow(v);
	}

	size_t offset = v->len * v->elem_size;
	memcpy(v->data + offset, data, v->elem_size);

	v->len++;
}

void vec_insert(struct vector* v, void* data, uint32_t index)
{
	if (v == NULL)
	{
		errno = EINVAL;
		return;
	}

	// process:
	// 1. reduce the length of this vec down to `index'
	// 2. push all elements from this point onwards back into the vec
	// the pushing accounts for growth caused by the insertion, but this method
	// copies all elements past `index' probably unnecessarily so there is room for improvement

	if (index > v->len)
	{
		errno = EINVAL;
		return;
	}

	// temporary array to store elements getting re-pushed
	// these all have to be copied because if the vec is grown then
	// the realloc will invalidate these pointers

	uint32_t tmplen = v->len - index;
	void** tmp = malloc(tmplen * sizeof(void*));

	int j = 0;
	for (int i = index; i < v->len; i++)
	{
		tmp[j] = vec_get_copied(v, i);
		j++;
	}

	v->len = index;
	vec_push(v, data);

	for (int i = 0; i < tmplen; i++)
	{
		vec_push(v, tmp[i]);
		free(tmp[i]);
	}

	free(tmp);
}

void vec_unshift(struct vector* v, void* data)
{
	vec_insert(v, data, 0);
}

void* vec_pop(struct vector* v)
{
	if (v == NULL)
	{
		errno = EINVAL;
		return NULL;
	}
	size_t offset = v->len * (v->elem_size - 1);
	void* out = malloc(v->elem_size);
	if (out == NULL)
	{
		errno = ENOMEM;
		return NULL;
	}
	memcpy(out, v->data + offset, v->elem_size);
	v->len--;
	return out;
}

void* vec_remove(struct vector* v, uint32_t index)
{
	// process:
	// 1: get relevant element as a copy
	// 2. memcpy all elements to the right, left by 1

	if (v == NULL)
	{
		errno = EINVAL;
		return NULL;
	}

	void* elem = vec_get_copied(v, index);
	for (int i = index; i < v->len; i++)
	{
		size_t curr_offset = (i+1) * v->elem_size;
		size_t new_offset = i * v->elem_size;
		memcpy(v->data + new_offset, v->data + curr_offset, v->elem_size);
	}
	v->len--;

	return elem;
}

void* vec_shift(struct vector* v)
{
	return vec_remove(v, 0);
}

void vec_swap(struct vector* v, uint32_t index1, uint32_t index2)
{
	if (v == NULL || index1 >= v->len || index2 >= v->len)
	{
		errno = EINVAL;
		return;
	} 

	if (index1 == index2)
	{
		return;
	}

	void* copy = vec_get_copied(v, index1);
	vec_set(v, vec_get(v, index2), index1);
	vec_set(v, copy, index2);
}

struct vector* vec_reverse(struct vector* v)
{
	if (v == NULL)
	{
		errno = EINVAL;
		return NULL;
	}

	struct vector* vc = vec_new_with_capacity(v->elem_size, v->capacity);
	if (vc == NULL)
	{
		errno = ENOMEM;
		return NULL;
	}

	for (int i = v->len - 1; i >= 0; i--)
	{
		vec_push(vc, vec_get(v, i));
	}

	return vc;
}

void vec_reverse_inplace(struct vector* v)
{
	if (v == NULL)
	{
		errno = EINVAL;
		return NULL;
	}

	// iterate through first half of vec to only swap elements once
	for (int i = 0; i < (int)floor((float)v->len / 2.0); i++)
	{
		uint32_t idx2 = v->len - i - 1;
		vec_swap(v, i, idx2);
	}
}

void vec_grow(struct vector* v)
{
	if (v == NULL)
	{
		errno = EINVAL;
		return 0;
	}

	char* new = realloc(v->data, v->capacity * v->elem_size * _VEC_CAPACITY_MULTIPLIER);
	if (new == NULL)
	{
		errno = ENOMEM;
		return;
	}

	v->capacity *= _VEC_CAPACITY_MULTIPLIER;
	v->data = new;
}

uint32_t vec_len(struct vector* v)
{
	if (v == NULL)
	{
		errno = EINVAL;
		return 0;
	}

	return v->len;
}

int vec_contains(struct vector* v, void* needle)
{
	if (v == NULL)
	{
		errno = EINVAL;
		return 0;
	}

	for (int i = 0; i < v->len; i += v->elem_size)
	{
		size_t offset = i * v->elem_size;
		int result = memcmp(v->data + offset, needle, v->elem_size);
		if (result == 0)
		{
			return 1;
		}
	}

	return 0;
}

struct vector* vec_map(struct vector* v, void* (*mapfn)(void*, uint32_t))
{
	if (v == NULL)
	{
		errno = EINVAL;
		return NULL;
	}

	struct vec* new_vec = vec_new_with_capacity(v->elem_size, v->capacity);
	for (int i = 0; i < v->len; i++)
	{
		void* elem = vec_get(v, i);
		void* new = (*mapfn)(elem, i);
		// invalid ret
		if (new == NULL)
		{
			errno = EFAULT;
			return NULL;
		}
		vec_push(new_vec, new);
		free(new);
	}

	return new_vec;
}

struct vector* vec_filter(struct vector* v, int (*filterfn)(void*, uint32_t))
{
	if (v == NULL)
	{
		errno = EINVAL;
		return NULL;
	}

	struct vec* new_vec = vec_new(v->elem_size);
	for (int i = 0; i < v->len; i++)
	{
		void* elem = vec_get(v, i);
		int include = (*filterfn)(elem, i);
		if(include)
			vec_push(new_vec, elem);
	}

	return new_vec;
}

void vec_foreach(struct vector* v, void (*foreachfn)(void*, uint32_t))
{
	if (v == NULL)
	{
		errno = EINVAL;
		return;
	}

	for (int i = 0; i < v->len; i++)
	{
		void* elem = vec_get_copied(v, i);
		(*foreachfn)(elem, i);
		free(elem);
	}
}

void vec_reset(struct vector* v)
{
	if (v == NULL)
	{
		errno = EINVAL;
		return;
	}
	// no need to clear memory - old elements overwritten as new ones are pushed
	v->len = 0;
}

void vec_destroy(struct vector* v)
{
	if (v == NULL)
	{
		errno = EINVAL;
		return;
	}
	free(v->data);
	free(v);
}