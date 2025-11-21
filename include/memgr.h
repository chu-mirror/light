#ifndef LIGHT_MEMGR_H
#define LIGHT_MEMGR_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>

extern size_t alloc_count;

#define NEW(p) (++alloc_count, assert((p) == NULL), (p) = (typeof(p))malloc(sizeof *(p)))
#define NEW_(p) ((p) = malloc(sizeof *(p)))

#define CALLOC(p, len)                                           \
    (++alloc_count, assert((p) == NULL), (p) = (typeof(p))calloc((len), sizeof *(p)))
#define CALLOC_(p, len) ((p) = (typeof(p))calloc((len), sizeof *(p)))

#define NEW0(p) CALLOC((p), 1)
#define NEW0_(p) CALLOC_((p), 1)

#define FREE(p)                                                       \
    ((void)(assert((p) != NULL), --alloc_count, free(p), (p) = NULL))
#define FREE_(p) ((void)(assert((p) != NULL), free(p), (p) = NULL))

#define REALLOC_ARRAY(p, len)                              \
    ((p) = (typeof(p))reallocarray((p), sizeof *(p), len))

#define MOVE(src, dest) (NEW(dest), memcpy((dest), (src), sizeof *(src)))
#define MOVE_ARRAY(src, dest, n)                                   \
    (CALLOC(dest, n), memcpy((dest), (src), sizeof *(src) * (n)))

#endif
