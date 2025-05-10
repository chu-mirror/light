#ifndef LIGHT_IDIOM_H
#define LIGHT_IDIOM_H

#include <stddef.h>

/* Give value a new name, and force type casting. */
#define RENAME(old, new) new = (typeof(new))old

/* Generic data structure usually accept pointers, enable them to handle
 * integers. */
#define VALUE(v) (void *)(ptrdiff_t)(v)

#endif
