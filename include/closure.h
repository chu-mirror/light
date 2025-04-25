#ifndef LIGHT_CLOSURE_H
#define LIGHT_CLOSURE_H

#include <stdint.h>
#include <stddef.h>

#include "light.h"

#define CLOSURE_NUMBER_MAX 100

typedef struct closure *Closure;

extern void *_light_closure_frame;
extern size_t closure_count;


void *closure(Closure);
Closure alloc_closure(void);
void init_closure(Closure cl, void *func, void *frame);
void free_closure(Closure);
size_t number_of_closures(void);

#define CLOSURE(cl) (*(typeof(cl)) closure((void *)(cl)))
#define NEW_CLOSURE(cl, func, fr) (++closure_count, (cl) = alloc_closure(), init_closure((cl), (func), (fr)))
#define FREE_CLOSURE(cl) (assert((cl) != NULL), --closure_count, free_closure(cl), (cl) = NULL)
#define INIT_CLOSURE_FRAME(fr) RENAME(_light_closure_frame, fr)

#endif
