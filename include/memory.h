#ifndef LIGHT_MEMORY_H
#define LIGHT_MEMORY_H

#include <stdlib.h>
#include <assert.h>

extern int alloc_count;

#define NEW(p) (++alloc_count, (p) = malloc(sizeof *(p)))
#define CALLOC(p, len) (++alloc_count, (p) = calloc((len), sizeof *(p)))
#define NEW0(p) CALLOC(p, 1)
#define FREE(p) ((void)(assert(p != NULL), --alloc_count, free(p), (p) = NULL))

#endif
