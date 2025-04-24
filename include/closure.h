#ifndef LIGHT_CLOSURE_H
#define LIGHT_CLOSURE_H

#include <stdint.h>

#include "idiom.h"

#define CLOSURE_NUMBER_MAX 100

typedef struct closure {
    void *func;
    void *frame;
} *Closure;

extern void *_light_closure_frame;
extern struct closure _light_closures[CLOSURE_NUMBER_MAX];
extern int _light_closures_tail;

static inline void init_closure(Closure cl, void *func, void *frame)
{
    cl->func = func;
    cl->frame = frame;
}

#define is_closure(cl) ((void *)(cl) >= (void *)&_light_closures[0] && (void *)cl <= (void *)&_light_closures[CLOSURE_NUMBER_MAX-1])
#define CLOSURE(cl) (*(typeof(cl))(is_closure(cl) ? \
	(_light_closure_frame = ((Closure)(cl))->frame, ((Closure)(cl))->func) : cl))
#define NEW_CLOSURE(cl, func, fr) \
    (cl = &_light_closures[_light_closures_tail %= CLOSURE_NUMBER_MAX, _light_closures_tail++], init_closure(cl, (func), (fr)))
#define INIT_CLOSURE_FRAME(fr) RENAME(_light_closure_frame, fr)

#endif
