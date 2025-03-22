#ifndef LIGHT_MEMORY_H
#define LIGHT_MEMORY_H

#include <stdlib.h>

#define NEW(p) ((p) = malloc(sizeof *(p)))
#define NEW0(p) ((p) = calloc(1, sizeof *(p)))
#define FREE(p) ((void)(free(p), (p) = NULL))

#endif
