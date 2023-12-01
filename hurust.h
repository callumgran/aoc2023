/*
 *  Copyright (C) 2023 Callum Gran
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
/*==========================================================================*

  FILE
    hurust.h

  PROJECT
    hurust generic library

  DESCRIPTION
    STB style header file for the hurust generic library.

  PROGRAMMER
    Callum Gran.

  MODIFICATIONS
    27-Nov-23  C.Gran		Created file.

 *==========================================================================*/

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#define swap(type, a, b)           \
    ({                             \
        register type _tmp = *(a); \
        *(a) = *(b);               \
        *(b) = _tmp;               \
    })

#define rotate(type, a, b, c)      \
    ({                             \
        register type _tmp = *(a); \
        *(a) = *(b);               \
        *(b) = *(c);               \
        *(c) = _tmp;               \
    })

#define _typeofarray(arr) typeof(*(arr))

#define _ensure_cap(structure)                                                                    \
    {                                                                                             \
        if ((structure)->size == (structure)->cap - 1) {                                          \
            (structure)->cap <<= 1;                                                               \
            (structure)->data =                                                                   \
                (structure)->allocator->realloc((structure)->allocator->arena, (structure)->data, \
                                                (structure)->cap * sizeof(*(structure)->data));   \
        }                                                                                         \
    }

#define _reduce_cap(structure)                                                                    \
    {                                                                                             \
        if ((structure)->size < (structure)->cap >> 2) {                                          \
            (structure)->cap >>= 1;                                                               \
            (structure)->data =                                                                   \
                (structure)->allocator->realloc((structure)->allocator->arena, (structure)->data, \
                                                (structure)->cap * sizeof(*(structure)->data));   \
        }                                                                                         \
    }

typedef void *(_alloc_fn_t)(void *arena, size_t size);

typedef void *(_realloc_fn_t)(void *arena, void *ptr, size_t size);

typedef void(_dealloc_fn_t)(void *arena, void *ptr);

typedef struct hr_allocator_t {
    void *arena;
    _alloc_fn_t *alloc;
    _realloc_fn_t *realloc;
    _dealloc_fn_t *dealloc;
} HRAllocator;

#define HR_ALLOCATOR_NO_ARENA(name, alloc_fn)           \
    void *_##name##_allocator(void *arena, size_t size) \
    {                                                   \
        (void)arena;                                    \
        return alloc_fn(size);                          \
    }

#define HR_REALLOCATOR_NO_ARENA(name, realloc_fn)                    \
    void *_##name##_reallocator(void *arena, void *ptr, size_t size) \
    {                                                                \
        (void)arena;                                                 \
        return realloc_fn(ptr, size);                                \
    }

#define HR_DEALLOCATOR_NO_ARENA(name, dealloc_fn)      \
    void _##name##_deallocator(void *arena, void *ptr) \
    {                                                  \
        (void)arena;                                   \
        dealloc_fn(ptr);                               \
    }

#define HR_ALLOCATOR_INIT(name, arena, alloc_fn, realloc_fn, dealloc_fn) \
    HRAllocator name = { arena, alloc_fn, realloc_fn, dealloc_fn };

#define HR_ALLOCATOR_NO_ARENA_INIT(name, alloc_fn, realloc_fn, dealloc_fn) \
    HR_ALLOCATOR_NO_ARENA(name, alloc_fn)                                  \
    HR_REALLOCATOR_NO_ARENA(name, realloc_fn)                              \
    HR_DEALLOCATOR_NO_ARENA(name, dealloc_fn)                              \
    HRAllocator name = { NULL, _##name##_allocator, _##name##_reallocator, _##name##_deallocator };

#define HR_SET_ALLOCATOR(allocator) hr_current_allocator = allocator

#define HR_RESET_ALLOCATOR() hr_current_allocator = &hr_default_allocator

#define HR_CUR_ALLOC(size) hr_current_allocator->alloc(hr_current_allocator->arena, (size))

#define HR_CUR_REALLOC(ptr, size) \
    hr_current_allocator->realloc(hr_current_allocator->arena, ptr, (size))

#define HR_CUR_DEALLOC(ptr) hr_current_allocator->dealloc(hr_current_allocator->arena, (ptr))

#define HR_ALLOC(allocator, size) (allocator)->alloc((allocator)->arena, (size))

#define HR_REALLOC(allocator, ptr, size) (allocator)->realloc((allocator)->arena, (ptr), (size))

#define HR_DEALLOC(allocator, ptr) (allocator)->dealloc((allocator)->arena, (ptr))

HR_ALLOCATOR_NO_ARENA_INIT(hr_default_allocator, malloc, realloc, free)

HRAllocator *hr_current_allocator = &hr_default_allocator;

#define HR_GLOBAL_ALLOCATOR hr_current_allocator

size_t hash_char(char key)
{
    return (size_t)key;
}

size_t hash_short(short key)
{
    return (size_t)key;
}

size_t hash_int(int key)
{
    return (size_t)key;
}

size_t hash_long(long key)
{
    return (size_t)key;
}

size_t hash_long_long(long long key)
{
    return (size_t)key;
}

size_t hash_unsigned_char(unsigned char key)
{
    return (size_t)key;
}

size_t hash_unsigned_short(unsigned short key)
{
    return (size_t)key;
}

size_t hash_unsigned_int(unsigned int key)
{
    return (size_t)key;
}

size_t hash_unsigned_long(unsigned long key)
{
    return (size_t)key;
}

size_t hash_unsigned_long_long(unsigned long long key)
{
    return (size_t)key;
}

size_t hash_float(float key)
{
    return *(size_t *)&key;
}

size_t hash_double(double key)
{
    return *(size_t *)&key;
}

size_t hash_long_double(long double key)
{
    return *(size_t *)&key;
}

size_t hash_str(const char *str)
{
    size_t A = 1327217885;
    size_t k = 0;
    for (size_t i = 0; i < strlen(str); i++)
        k += (k << 5) + str[i];

    return k * A;
}

#define HURUST_IMPLEMENTATION

#ifdef HURUST_IMPLEMENTATION
#define HURUST_DYNAMIC_IMPLEMENTATION
#define HURUST_STATIC_IMPLEMENTATION
#define HURUST_FUNCTIONAL_LAMBDA_H
#endif

#ifdef HURUST_STATIC_IMPLEMENTATION
#define HURUST_STATIC_QUEUE_H
#define HURUST_STATIC_STACK_H
#define HURUST_ARRAY_H
#define HURUST_STATIC_HASHSET_H
#endif

#ifdef HURUST_DYNAMIC_IMPLEMENTATION
#define HURUST_DYNAMIC_QUEUE_H
#define HURUST_DYNAMIC_STACK_H
#define HURUST_VECTOR_H
#define HURUST_HEAP_H
#endif

#if defined(HURUST_ARRAY_H) || defined(HURUST_VECTOR_H)
#define HURUST_SORT_H
#endif

#ifdef HURUST_SORT_H

#define INSERTION_SORT_THRESHOLD 27

#define _median_three(left, right, cmp)                       \
    ({                                                        \
        _type *_med_mid = (left) + (((right) - (left)) >> 1); \
        if ((cmp)(*_med_mid, *(left)) < 0) {                  \
            if ((cmp)(*(right), *(left)) < 0) {               \
                if ((cmp)(*(right), *_med_mid) < 0) {         \
                    swap(_type, (left), (right));             \
                } else {                                      \
                    rotate(_type, (left), _med_mid, (right)); \
                }                                             \
            } else {                                          \
                swap(_type, (left), _med_mid);                \
            }                                                 \
        } else {                                              \
            if ((cmp)(*(right), *_med_mid) < 0) {             \
                if ((cmp)(*(right), *(left)) < 0) {           \
                    rotate(_type, (right), _med_mid, (left)); \
                } else {                                      \
                    swap(_type, _med_mid, (right));           \
                }                                             \
            }                                                 \
        }                                                     \
        _med_mid;                                             \
    })

#define _partition(left, right, cmp)                         \
    ({                                                       \
        _type *_p_mid = _median_three((left), (right), cmp); \
        const _type _piv = *_p_mid;                          \
        _type *_l = (left) + 1;                              \
        _type *_r = (right);                                 \
        *_p_mid = *_l;                                       \
        *_l = _piv;                                          \
        swap(_type, _p_mid, (right)-1);                      \
        do {                                                 \
            do                                               \
                _l++;                                        \
            while ((cmp)(*_l, _piv) < 0);                    \
            do                                               \
                _r--;                                        \
            while ((cmp)(_piv, *_r) < 0);                    \
            if (_l >= _r)                                    \
                break;                                       \
            swap(_type, _l, _r);                             \
        } while (true);                                      \
        *((left) + 1) = *_r;                                 \
        *_r = _piv;                                          \
        _r;                                                  \
    })

#define sort(arr, size, cmp)                                      \
    ({                                                            \
        typedef _typeofarray((arr)) _type;                        \
        _type *_left = (arr);                                     \
        _type *_right = (arr) + (size)-1;                         \
        struct _qs_stack {                                        \
            _type *left;                                          \
            _type *right;                                         \
        } _stack[sizeof(size_t) * 8];                             \
        struct _qs_stack *_top = _stack;                          \
        _top->left = _left;                                       \
        _top->right = _right;                                     \
        _top++;                                                   \
        do {                                                      \
            if (_right - _left <= INSERTION_SORT_THRESHOLD) {     \
                for (_type *_i = _left + 1; _i <= _right; _i++)   \
                    if (cmp(*_i, *_left) < 0)                     \
                        swap(_type, _i, _left);                   \
                for (_type *_i = _left + 2; _i <= _right; _i++) { \
                    _type _high = *_i;                            \
                    _type *_j = _i - 1;                           \
                    for (; cmp(_high, *(_j)) < 0; _j--)           \
                        *(_j + 1) = *_j;                          \
                    *(_j + 1) = _high;                            \
                }                                                 \
                _top--;                                           \
                _left = _top->left;                               \
                _right = _top->right;                             \
            } else {                                              \
                _type *_mid = _partition(_left, _right, cmp);     \
                if (_mid - _left >= _right - _mid) {              \
                    _top->left = _left;                           \
                    _top->right = _mid - 1;                       \
                    _top++;                                       \
                    _left = _mid + 1;                             \
                } else {                                          \
                    _top->left = _mid + 1;                        \
                    _top->right = _right;                         \
                    _top++;                                       \
                    _right = _mid - 1;                            \
                }                                                 \
            }                                                     \
        } while (_top > _stack);                                  \
    })

#endif

#if defined(HURUST_FUNCTIONAL_LAMBDA_H) && defined(__GNUC__)

#define lambda(return_type, func_args, func_body) \
    ({ return_type __fn_t__ func_args func_body &__fn_t__; })

#define anon(return_type, func_args, func_body, ...) \
    lambda(return_type, func_args, func_body)(__VA_ARGS__)

#endif

#ifdef HURUST_STATIC_STACK_H

#define SSTACK(type, struct_prefix)           \
    typedef struct struct_prefix##_sstack_t { \
        type *data;                           \
        size_t size;                          \
        size_t cap;                           \
        struct hr_allocator_t *allocator;     \
    } struct_prefix##_sstack_t;

#define sstack_init(_stack, _allocator, _cap)                                                    \
    ({                                                                                           \
        (_stack)->allocator = (_allocator);                                                      \
        (_stack)->cap = (_cap);                                                                  \
        (_stack)->size = 0;                                                                      \
        (_stack)->data = HR_ALLOC((_stack)->allocator, sizeof(*(_stack)->data) * (_stack)->cap); \
    })

#define sstack_free(_stack) ({ HR_DEALLOC((_stack)->allocator, (_stack)->data); })

#define sstack_get_size(_stack) ({ (_stack)->size; })

#define sstack_get_cap(_stack) ({ (_stack)->cap; })

#define sstack_get_data(_stack) ({ (_stack)->data; })

#define sstack_get_allocator(_stack) ({ (_stack)->allocator; })

#define sstack_set_size(_stack, _size) ({ (_stack)->size = (_size); })

#define sstack_set_cap(_stack, _cap) ({ (_stack)->cap = (_cap); })

#define sstack_set_data(_stack, _data) ({ (_stack)->data = (_data); })

#define sstack_set_allocator(_stack, _allocator) ({ (_stack)->allocator = (_allocator); })

#define sstack_full(_stack) ({ (_stack)->size == (_stack)->cap; })

#define sstack_empty(_stack) ({ (_stack)->size == 0; })

#define sstack_pop(_stack)              \
    ({                                  \
        (_stack)->size--;               \
        (_stack)->data[(_stack)->size]; \
    })

#define sstack_push(_stack, _item)                 \
    ({                                             \
        (_stack)->data[(_stack)->size] = *(_item); \
        (_stack)->size++;                          \
    })

#define sstack_peek(_stack) ({ (_stack)->data[(_stack)->size - 1]; })

#endif

#ifdef HURUST_DYNAMIC_STACK_H

#define DSTACK(type, struct_prefix)           \
    typedef struct struct_prefix##_dstack_t { \
        type *data;                           \
        size_t size;                          \
        size_t cap;                           \
        struct hr_allocator_t *allocator;     \
    } struct_prefix##_dstack_t;

#define dstack_init(_stack, _allocator, _cap)                                                    \
    ({                                                                                           \
        (_stack)->allocator = (_allocator);                                                      \
        (_stack)->cap = (_cap);                                                                  \
        (_stack)->size = 0;                                                                      \
        (_stack)->data = HR_ALLOC((_stack)->allocator, sizeof(*(_stack)->data) * (_stack)->cap); \
    })

#define dstack_free(_stack) ({ HR_DEALLOC((_stack)->allocator, (_stack)->data); })

#define dstack_get_size(_stack) ({ (_stack)->size; })

#define dstack_get_cap(_stack) ({ (_stack)->cap; })

#define dstack_get_data(_stack) ({ (_stack)->data; })

#define dstack_get_allocator(_stack) ({ (_stack)->allocator; })

#define dstack_set_size(_stack, _size) ({ (_stack)->size = (_size); })

#define dstack_set_cap(_stack, _cap) ({ (_stack)->cap = (_cap); })

#define dstack_set_data(_stack, _data) ({ (_stack)->data = (_data); })

#define dstack_set_allocator(_stack, _allocator) ({ (_stack)->allocator = (_allocator); })

#define dstack_empty(_stack) ({ (_stack)->size == 0; })

#define dstack_pop(_stack)                                                  \
    ({                                                                      \
        (_stack)->size--;                                                   \
        _typeofarray((_stack)->data) _ret = (_stack)->data[(_stack)->size]; \
        _reduce_cap((_stack));                                              \
        _ret;                                                               \
    })

#define dstack_push(_stack, _item)                 \
    ({                                             \
        _ensure_cap((_stack));                     \
        (_stack)->data[(_stack)->size] = *(_item); \
        (_stack)->size++;                          \
    })

#define dstack_peek(_stack) ({ (_stack)->data[(_stack)->size - 1]; })

#endif

#ifdef HURUST_STATIC_QUEUE_H

#define SQUEUE(type, struct_prefix)           \
    typedef struct struct_prefix##_squeue_t { \
        type *data;                           \
        size_t start;                         \
        size_t end;                           \
        size_t size;                          \
        size_t cap;                           \
        struct hr_allocator_t *allocator;     \
    } struct_prefix##_squeue_t;

#define squeue_init(_queue, _allocator, _cap)                                                    \
    ({                                                                                           \
        (_queue)->allocator = (_allocator);                                                      \
        (_queue)->cap = (_cap);                                                                  \
        (_queue)->size = 0;                                                                      \
        (_queue)->start = 0;                                                                     \
        (_queue)->end = 0;                                                                       \
        (_queue)->data = HR_ALLOC((_queue)->allocator, sizeof(*(_queue)->data) * (_queue)->cap); \
    })

#define squeue_free(_queue) ({ HR_DEALLOC((_queue)->allocator, (_queue)->data); })

#define squeue_get_size(_queue) ({ (_queue)->size; })

#define squeue_get_cap(_queue) ({ (_queue)->cap; })

#define squeue_get_data(_queue) ({ (_queue)->data; })

#define squeue_get_allocator(_queue) ({ (_queue)->allocator; })

#define squeue_get_start(_queue) ({ (_queue)->start; })

#define squeue_get_end(_queue) ({ (_queue)->end; })

#define squeue_set_size(_queue, _size) ({ (_queue)->size = (_size); })

#define squeue_set_cap(_queue, _cap) ({ (_queue)->cap = (_cap); })

#define squeue_set_data(_queue, _data) ({ (_queue)->data = (_data); })

#define squeue_set_allocator(_queue, _allocator) ({ (_queue)->allocator = (_allocator); })

#define squeue_set_start(_queue, _start) ({ (_queue)->start = (_start); })

#define squeue_set_end(_queue, _end) ({ (_queue)->end = (_end); })

#define squeue_full(_queue) ({ (_queue)->size == (_queue)->cap; })

#define squeue_empty(_queue) ({ (_queue)->size == 0; })

#define squeue_pop(_queue)                                                   \
    ({                                                                       \
        _typeofarray((_queue)->data) _ret = (_queue)->data[(_queue)->start]; \
        (_queue)->start = ((_queue)->start + 1) % (_queue)->cap;             \
        (_queue)->size--;                                                    \
        _ret;                                                                \
    })

#define squeue_push(_queue, _item)                           \
    ({                                                       \
        (_queue)->data[(_queue)->end] = *(_item);            \
        (_queue)->end = ((_queue)->end + 1) % (_queue)->cap; \
        (_queue)->size++;                                    \
    })

#define squeue_peek(_queue) ({ (_queue)->data[(_queue)->start]; })

#endif

#ifdef HURUST_DYNAMIC_QUEUE_H

#define DQUEUE(type, struct_prefix)           \
    typedef struct struct_prefix##_dqueue_t { \
        type *data;                           \
        size_t start;                         \
        size_t end;                           \
        size_t size;                          \
        size_t cap;                           \
        struct hr_allocator_t *allocator;     \
    } struct_prefix##_dqueue_t;

#define dqueue_init(_queue, _allocator, _cap)                                                    \
    ({                                                                                           \
        (_queue)->allocator = (_allocator);                                                      \
        (_queue)->cap = (_cap);                                                                  \
        (_queue)->size = 0;                                                                      \
        (_queue)->start = 0;                                                                     \
        (_queue)->end = 0;                                                                       \
        (_queue)->data = HR_ALLOC((_queue)->allocator, sizeof(*(_queue)->data) * (_queue)->cap); \
    })

#define dqueue_free(_queue) ({ HR_DEALLOC((_queue)->allocator, (_queue)->data); })

#define dqueue_get_size(_queue) ({ (_queue)->size; })

#define dqueue_get_cap(_queue) ({ (_queue)->cap; })

#define dqueue_get_data(_queue) ({ (_queue)->data; })

#define dqueue_get_allocator(_queue) ({ (_queue)->allocator; })

#define dqueue_get_start(_queue) ({ (_queue)->start; })

#define dqueue_get_end(_queue) ({ (_queue)->end; })

#define dqueue_set_size(_queue, _size) ({ (_queue)->size = (_size); })

#define dqueue_set_cap(_queue, _cap) ({ (_queue)->cap = (_cap); })

#define dqueue_set_data(_queue, _data) ({ (_queue)->data = (_data); })

#define dqueue_set_allocator(_queue, _allocator) ({ (_queue)->allocator = (_allocator); })

#define dqueue_set_start(_queue, _start) ({ (_queue)->start = (_start); })

#define dqueue_set_end(_queue, _end) ({ (_queue)->end = (_end); })

#define dqueue_empty(_queue) ({ (_queue)->size == 0; })

#define dqueue_pop(_queue)                                                   \
    ({                                                                       \
        _typeofarray((_queue)->data) _ret = (_queue)->data[(_queue)->start]; \
        (_queue)->start = ((_queue)->start + 1) % (_queue)->cap;             \
        (_queue)->size--;                                                    \
        _ret;                                                                \
    })

#define dqueue_push(_queue, _item)                                                \
    ({                                                                            \
        if ((_queue)->size == (_queue)->cap) {                                    \
            (_queue)->cap *= 2;                                                   \
            (_queue)->data = HR_REALLOC((_queue)->allocator, (_queue)->data,      \
                                        sizeof(*(_queue)->data) * (_queue)->cap); \
            if ((_queue)->start >= (_queue)->end) {                               \
                size_t i = 0;                                                     \
                for (i = 0; i < (_queue)->end; i++) {                             \
                    (_queue)->data[i + (_queue)->size] = (_queue)->data[i];       \
                }                                                                 \
                (_queue)->end += (_queue)->size;                                  \
            }                                                                     \
        }                                                                         \
        (_queue)->data[(_queue)->end] = *(_item);                                 \
        (_queue)->end = ((_queue)->end + 1) % (_queue)->cap;                      \
        (_queue)->size++;                                                         \
    })

#define dqueue_peek(_queue) ({ (_queue)->data[(_queue)->start]; })

#endif

#ifdef HURUST_ARRAY_H

#define ARRAY(type, struct_prefix)           \
    typedef struct struct_prefix##_array_t { \
        type *data;                          \
        size_t size;                         \
        size_t cap;                          \
        int (*cmp)(const type, const type);  \
        struct hr_allocator_t *allocator;    \
    } struct_prefix##_array_t;

#define array_init(_array, _allocator, _cap, _cmp)                                               \
    ({                                                                                           \
        (_array)->allocator = (_allocator);                                                      \
        (_array)->cap = (_cap);                                                                  \
        (_array)->size = 0;                                                                      \
        (_array)->cmp = (_cmp);                                                                  \
        (_array)->data = HR_ALLOC((_array)->allocator, sizeof(*(_array)->data) * (_array)->cap); \
    })

#define array_free(_array) ({ HR_DEALLOC((_array)->allocator, (_array)->data); })

#define array_get_size(_array) ({ (_array)->size; })

#define array_get_cap(_array) ({ (_array)->cap; })

#define array_get_data(_array) ({ (_array)->data; })

#define array_get_cmp(_array) ({ (_array)->cmp; })

#define array_get_allocator(_array) ({ (_array)->cmp; })

#define array_set_size(_array, _size) ({ (_array)->size = (_size); })

#define array_set_cap(_array, _cap) ({ (_array)->cap = (_cap); })

#define array_set_data(_array, _data) ({ (_array)->data = (_data); })

#define array_set_cmp(_array, _cmp) ({ (_array)->cmp = (_cmp); })

#define array_set_allocator(_array, _allocator) ({ (_array)->allocator = (_allocator); })

#define array_full(_array) ({ (_array)->size == (_array)->cap; })

#define array_empty(_array) ({ (_array)->size == 0; })

#define array_pop(_array, _i)                                     \
    ({                                                            \
        _typeofarray((_array)->data) _ret = (_array)->data[(_i)]; \
        (_array)->size--;                                         \
        for (size_t _j = (_i); _j < (_array)->size; _j++) {       \
            (_array)->data[_j] = (_array)->data[_j + 1];          \
        }                                                         \
        _ret;                                                     \
    })

#define array_remove(_array, _item)                              \
    ({                                                           \
        size_t _i = 0;                                           \
        for (; _i < (_array)->size; _i++)                        \
            if ((_array)->cmp((_array)->data[_i], (_item)) == 0) \
                break;                                           \
        array_pop(_array, _i);                                   \
    })

#define array_get(_array, _i) ({ (_array)->data[(_i)]; })

#define array_set(_array, _item, _i) ({ (_array)->data[(_i)] = *(_item); })

#define array_push(_array, _item)                  \
    ({                                             \
        (_array)->data[(_array)->size] = *(_item); \
        (_array)->size++;                          \
    })

#define array_sort(_array) ({ sort((_array)->data, (_array)->size, (_array)->cmp); })

#define array_max(_array)                                      \
    ({                                                         \
        _typeofarray((_array)->data) _ret = (_array)->data[0]; \
        for (size_t _i = 1; _i < (_array)->size; _i++) {       \
            if ((_array)->cmp(_ret, (_array)->data[_i]) < 0)   \
                _ret = (_array)->data[_i];                     \
        }                                                      \
        _ret;                                                  \
    })

#define array_min(_array)                                      \
    ({                                                         \
        _typeofarray((_array)->data) _ret = (_array)->data[0]; \
        for (size_t _i = 1; _i < (_array)->size; _i++) {       \
            if ((_array)->cmp(_ret, (_array)->data[_i]) > 0)   \
                _ret = (_array)->data[_i];                     \
        }                                                      \
        _ret;                                                  \
    })

#define array_foreach(_array, _func)                     \
    ({                                                   \
        for (size_t _i = 0; _i < (_array)->size; _i++) { \
            _func(&(_array)->data[_i]);                  \
        }                                                \
    })

#endif

#ifdef HURUST_VECTOR_H

#define VECTOR(type, struct_prefix)           \
    typedef struct struct_prefix##_vector_t { \
        type *data;                           \
        size_t size;                          \
        size_t cap;                           \
        int (*cmp)(const type, const type);   \
        struct hr_allocator_t *allocator;     \
    } struct_prefix##_vector_t;

#define vector_init(_vector, _allocator, _cap, _cmp)                                   \
    ({                                                                                 \
        (_vector)->allocator = (_allocator);                                           \
        (_vector)->cap = (_cap);                                                       \
        (_vector)->size = 0;                                                           \
        (_vector)->cmp = (_cmp);                                                       \
        (_vector)->data =                                                              \
            HR_ALLOC((_vector)->allocator, sizeof(*(_vector)->data) * (_vector)->cap); \
    })

#define vector_free(_vector) ({ HR_DEALLOC((_vector)->allocator, (_vector)->data); })

#define vector_get_size(_vector) ({ (_vector)->size; })

#define vector_get_cap(_vector) ({ (_vector)->cap; })

#define vector_get_data(_vector) ({ (_vector)->data; })

#define vector_get_cmp(_vector) ({ (_vector)->cmp; })

#define vector_get_allocator(_vector) ({ (_vector)->cmp; })

#define vector_set_size(_vector, _size) ({ (_vector)->size = (_size); })

#define vector_set_cap(_vector, _cap) ({ (_vector)->cap = (_cap); })

#define vector_set_data(_vector, _data) ({ (_vector)->data = (_data); })

#define vector_set_cmp(_vector, _cmp) ({ (_vector)->cmp = (_cmp); })

#define vector_set_allocator(_vector, _allocator) ({ (_vector)->allocator = (_allocator); })

#define vector_empty(_vector) ({ (_vector)->size == 0; })

#define vector_pop(_vector, _i)                                     \
    ({                                                              \
        _typeofarray((_vector)->data) _ret = (_vector)->data[(_i)]; \
        (_vector)->size--;                                          \
        for (size_t _j = (_i); _j < (_vector)->size; _j++) {        \
            (_vector)->data[_j] = (_vector)->data[_j + 1];          \
        }                                                           \
        _reduce_cap(_vector);                                       \
        _ret;                                                       \
    })

#define vector_remove(_vector, _item)                              \
    ({                                                             \
        size_t _i = 0;                                             \
        for (; _i < (_vector)->size; _i++)                         \
            if ((_vector)->cmp((_vector)->data[_i], (_item)) == 0) \
                break;                                             \
        vector_pop(_vector, _i);                                   \
    })

#define vector_get(_vector, _i) ({ (_vector)->data[(_i)]; })

#define vector_set(_vector, _item, _i) ({ (_vector)->data[(_i)] = *(_item); })

#define vector_push(_vector, _item)                  \
    ({                                               \
        _ensure_cap(_vector);                        \
        (_vector)->data[(_vector)->size] = *(_item); \
        (_vector)->size++;                           \
    })

#define vector_sort(_vector) ({ sort((_vector)->data, (_vector)->size, (_vector)->cmp); })

#define vector_max(_vector)                                      \
    ({                                                           \
        _typeofarray((_vector)->data) _ret = (_vector)->data[0]; \
        for (size_t _i = 1; _i < (_vector)->size; _i++) {        \
            if ((_vector)->cmp(_ret, (_vector)->data[_i]) < 0)   \
                _ret = (_vector)->data[_i];                      \
        }                                                        \
        _ret;                                                    \
    })

#define vector_min(_vector)                                      \
    ({                                                           \
        _typeofarray((_vector)->data) _ret = (_vector)->data[0]; \
        for (size_t _i = 1; _i < (_vector)->size; _i++) {        \
            if ((_vector)->cmp(_ret, (_vector)->data[_i]) > 0)   \
                _ret = (_vector)->data[_i];                      \
        }                                                        \
        _ret;                                                    \
    })

#define vector_foreach(_vector, _func)                    \
    ({                                                    \
        for (size_t _i = 0; _i < (_vector)->size; _i++) { \
            _func(&(_vector)->data[_i]);                  \
        }                                                 \
    })

#endif

#ifdef HURUST_HEAP_H

#define HEAP(type, struct_prefix)           \
    typedef struct struct_prefix##_heap_t { \
        type *data;                         \
        size_t size;                        \
        size_t cap;                         \
        int (*cmp)(const type, const type); \
        struct hr_allocator_t *allocator;   \
    } struct_prefix##_heap_t;

#define heap_init(_heap, _allocator, _cap, _cmp)                                             \
    ({                                                                                       \
        (_heap)->allocator = (_allocator);                                                   \
        (_heap)->cap = (_cap);                                                               \
        (_heap)->size = 0;                                                                   \
        (_heap)->cmp = (_cmp);                                                               \
        (_heap)->data = HR_ALLOC((_heap)->allocator, sizeof(*(_heap)->data) * (_heap)->cap); \
    })

#define heap_free(_heap) ({ HR_DEALLOC((_heap)->allocator, (_heap)->data); })

#define _heap_left_child_idx(_parent_idx) (((_parent_idx) << 1) + 1)
#define _heap_right_child_idx(_parent_idx) (((_parent_idx) + 1) << 1)
#define _heap_parent_idx(_child_idx) (((_child_idx)-1) >> 1)

#define _heap_has_left_child(_heap, _parent_idx) \
    (_heap_left_child_idx(_parent_idx) < (__ssize_t)(_heap)->size)

#define _heap_has_right_child(_heap, _parent_idx) \
    (_heap_right_child_idx(_parent_idx) < (__ssize_t)(_heap)->size)

#define heap_left_child(_heap, _parent_idx) ((_heap)->data[_heap_left_child_idx(_parent_idx)])

#define heap_right_child(_heap, _parent_idx) ((_heap)->data[_heap_right_child_idx(_parent_idx)])

#define heap_parent(_heap, _child_idx) ((_heap)->data[_heap_parent_idx(_child_idx)])

#define _heapify_up(_heap)                                                            \
    ({                                                                                \
        __ssize_t _idx = (_heap)->size - 1;                                           \
        __ssize_t _parent_idx = _heap_parent_idx(_idx);                               \
        while (_parent_idx >= 0 &&                                                    \
               (_heap)->cmp(((_heap)->data[_idx]), (_heap)->data[_parent_idx]) < 0) { \
            swap(_typeofarray((_heap)->data), ((_heap)->data + _parent_idx),          \
                 ((_heap)->data + _idx));                                             \
            _idx = _parent_idx;                                                       \
            _parent_idx = _heap_parent_idx(_idx);                                     \
        }                                                                             \
    })

#define _heapify_down(_heap)                                                                       \
    ({                                                                                             \
        __ssize_t _idx = 0;                                                                        \
        __ssize_t _min_idx;                                                                        \
        while (_heap_has_left_child(_heap, _idx)) {                                                \
            _min_idx = _heap_left_child_idx(_idx);                                                 \
            if (_heap_has_right_child(_heap, _idx) &&                                              \
                (_heap)->cmp(heap_right_child(_heap, _idx), heap_left_child(_heap, _idx)) < 0) {   \
                _min_idx = _heap_right_child_idx(_idx);                                            \
            }                                                                                      \
            if ((_heap)->cmp(((_heap)->data[_min_idx]), (_heap)->data[_idx]) > 0) {                \
                break;                                                                             \
            } else {                                                                               \
                swap(_typeofarray((_heap)->data), (_heap)->data + _idx, (_heap)->data + _min_idx); \
                _idx = _min_idx;                                                                   \
            }                                                                                      \
        }                                                                                          \
    })

#define heap_get_size(_heap) ({ (_heap)->size; })

#define heap_get_cap(_heap) ({ (_heap)->cap; })

#define heap_get_data(_heap) ({ (_heap)->data; })

#define heap_get_cmp(_heap) ({ (_heap)->cmp; })

#define heap_get_allocator(_heap) ({ (_heap)->cmp; })

#define heap_set_size(_heap, _size) ({ (_heap)->size = (_size); })

#define heap_set_cap(_heap, _cap) ({ (_heap)->cap = (_cap); })

#define heap_set_data(_heap, _data) ({ (_heap)->data = (_data); })

#define heap_set_cmp(_heap, _cmp) ({ (_heap)->cmp = (_cmp); })

#define heap_set_allocator(_heap, _allocator) ({ (_heap)->allocator = (_allocator); })

#define heap_empty(_heap) ({ (_heap)->size == 0; })

#define heap_push(_heap, _item)                    \
    ({                                             \
        _ensure_cap(_heap);                        \
        (_heap)->data[(_heap)->size++] = *(_item); \
        _heapify_up(_heap);                        \
    })

#define heap_pop(_heap)                                      \
    ({                                                       \
        _typeofarray((_heap)->data) _ret = (_heap)->data[0]; \
        (_heap)->data[0] = (_heap)->data[--(_heap)->size];   \
        _heapify_down(_heap);                                \
        _reduce_cap(_heap);                                  \
        _ret;                                                \
    })

#define heap_peek(_heap) ({ (_heap)->data[0]; })

#endif

#ifdef HURUST_STATIC_HASHSET_H
static bool is_prime(size_t n)
{
    if (n <= 1) {
        return false;
    }

    if (n <= 3) {
        return true;
    }

    if (n % 2 == 0 || n % 3 == 0) {
        return false;
    }

    for (size_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }

    return true;
}

#define SHASHSET(type, struct_prefix)           \
    typedef struct struct_prefix##_shashset_t { \
        type *data;                             \
        size_t size;                            \
        size_t cap;                             \
        int (*cmp)(const type a, const type b); \
        size_t (*hash)(const type a);           \
        struct hr_allocator_t *allocator;       \
    } struct_prefix##_shashset_t;

#define shashset_init(_hashset, _allocator, _cap, _cmp, _hash)                            \
    ({                                                                                    \
        size_t __cap = (_cap);                                                            \
        (_hashset)->allocator = (_allocator);                                             \
        while (!is_prime(__cap)) {                                                        \
            __cap++;                                                                      \
        }                                                                                 \
        (_hashset)->cap = __cap;                                                          \
        (_hashset)->size = 0;                                                             \
        (_hashset)->cmp = (_cmp);                                                         \
        (_hashset)->hash = (_hash);                                                       \
        (_hashset)->data =                                                                \
            HR_ALLOC((_hashset)->allocator, sizeof(*(_hashset)->data) * (_hashset)->cap); \
    })

#define shashset_free(_hashset) HR_DEALLOC((_hashset)->allocator, (_hashset)->data)

#define shashset_insert(_hashset, _item)                             \
    ({                                                               \
        size_t _hash = (_hashset)->hash(_item);                      \
        size_t _index = _hash % (_hashset)->cap;                     \
        size_t _i = _index;                                          \
        bool _success = true;                                        \
        while ((_hashset)->data[_i] != NULL_VAL(_item)) {            \
            if ((_hashset)->cmp((_hashset)->data[_i], _item) == 0) { \
                break;                                               \
            }                                                        \
            _i = (_i + 1) % (_hashset)->cap;                         \
            if (_i == _index) {                                      \
                _success = false;                                    \
                break;                                               \
            }                                                        \
        }                                                            \
        if (_success) {                                              \
            (_hashset)->data[_i] = _item;                            \
            (_hashset)->size++;                                      \
        }                                                            \
        _success;                                                    \
    })

#define shashset_remove(_hashset, _item)                             \
    ({                                                               \
        size_t _hash = (_hashset)->hash(_item);                      \
        size_t _index = _hash % (_hashset)->cap;                     \
        size_t _i = _index;                                          \
        bool _success = false;                                       \
        while ((_hashset)->data[_i] != NULL_VAL(_item)) {            \
            if ((_hashset)->cmp((_hashset)->data[_i], _item) == 0) { \
                (_hashset)->data[_i] = NULL_VAL(_item);              \
                (_hashset)->size--;                                  \
                _success = true;                                     \
                break;                                               \
            }                                                        \
            _i = (_i + 1) % (_hashset)->cap;                         \
            if (_i == _index) {                                      \
                break;                                               \
            }                                                        \
        }                                                            \
        _success;                                                    \
    })

#define shashset_contains(_hashset, _item)                           \
    ({                                                               \
        size_t _hash = (_hashset)->hash(_item);                      \
        size_t _index = _hash % (_hashset)->cap;                     \
        size_t _i = _index;                                          \
        bool _success = false;                                       \
        while ((_hashset)->data[_i] != NULL_VAL(_item)) {            \
            if ((_hashset)->cmp((_hashset)->data[_i], _item) == 0) { \
                _success = true;                                     \
                break;                                               \
            }                                                        \
            _i = (_i + 1) % (_hashset)->cap;                         \
            if (_i == _index) {                                      \
                break;                                               \
            }                                                        \
        }                                                            \
        _success;                                                    \
    })

#define shashset_size(_hashset) ((_hashset)->size)

#define shashset_empty(_hashset) ((_hashset)->size == 0)

#define shashset_capacity(_hashset) ((_hashset)->cap)

#define shashset_load_factor(_hashset) ((float)(_hashset)->size / (_hashset)->cap)

#endif