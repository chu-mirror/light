#include "closure.h"

void *_light_closure_frame;
struct closure _light_closures[CLOSURE_NUMBER_MAX];
int _light_closures_tail;
