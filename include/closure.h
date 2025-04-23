#ifndef LIGHT_CLOSURE_H
#define LIGHT_CLOSURE_H

#include <stdint.h>

#include "idiom.h"

#define CLOSURE_MAGIC_NUMBER 0x434C4F53

typedef struct closure {
    uint32_t magic;
    void *func;
    void *frame;
} *Closure;

extern void *_light_closure_frame;

static inline void init_closure(Closure cl, void *func, void *frame)
{
    cl->magic = CLOSURE_MAGIC_NUMBER;
    cl->func = func;
    cl->frame = frame;
}

#define CLOSURE(cl) (*(typeof(cl))(		     \
    ((Closure)(cl))->magic == CLOSURE_MAGIC_NUMBER ? \
	(_light_closure_frame = ((Closure)(cl))->frame, ((Closure)(cl))->func) : cl))
#define NEW_CLOSURE(cl, func, fr) (NEW(cl), init_closure(cl, (func), (fr)))
#define INIT_CLOSURE_FRAME(fr) RENAME(_light_closure_frame, fr)

#endif
