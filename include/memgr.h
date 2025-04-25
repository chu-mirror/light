#ifndef LIGHT_MEMGR_H
#define LIGHT_MEMGR_H

#include <stdlib.h>
#include <assert.h>

extern size_t alloc_count;

#define NEW(p) (++alloc_count, (p) = malloc(sizeof *(p)))
#define _NEW(p) ((p) = malloc(sizeof *(p)))

#define CALLOC(p, len) (++alloc_count, (p) = calloc((len), sizeof *(p)))
#define _CALLOC(p, len) ((p) = calloc((len), sizeof *(p)))

#define NEW0(p) CALLOC(p, 1)
#define _NEW0(p) _CALLOC(p, 1)

#define FREE(p) ((void)(assert(p != NULL), --alloc_count, free(p), (p) = NULL))
#define _FREE(p) ((void)(assert(p != NULL), free(p), (p) = NULL))

#define REALLOC_ARRAY(p, len) (p = reallocarray(p, sizeof *(p), len))

#endif
