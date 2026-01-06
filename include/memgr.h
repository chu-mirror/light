#ifndef LIGHT_MEMGR_H
#define LIGHT_MEMGR_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>

extern size_t alloc_count;
extern size_t reserved_count;

#define assert_memory_safety() assert(alloc_count == reserved_count)

#define NEW(p) \
    (++alloc_count, assert((p) == NULL), (p) = (typeof(p))malloc(sizeof *(p)))

#define CALLOC(p, len)                            \
    (++alloc_count,                               \
     assert((p) == NULL),                         \
     (p) = (typeof(p))calloc((len), sizeof *(p)))

#define NEW0(p) CALLOC((p), 1)

#define FREE(p)                                                       \
    ((void)(assert((p) != NULL), --alloc_count, free(p), (p) = NULL))

#define REALLOC_ARRAY(p, len)                              \
    ((p) = (typeof(p))reallocarray((p), sizeof *(p), len))

#define MOVE(src, dest) (NEW(dest), memcpy((dest), (src), sizeof *(src)))
#define MOVE_ARRAY(src, dest, n)                                  \
    (CALLOC(dest, n), memcpy((dest), (src), sizeof *(src) * (n)))

#endif
