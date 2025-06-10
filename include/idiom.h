#ifndef LIGHT_IDIOM_H
#define LIGHT_IDIOM_H

#include <stddef.h>
#include <string.h>

/* Give value a new name, and force type casting. */
#define RENAME(old, new) new = (typeof(new))old

/* Generic data structure usually accept pointers, enable them to handle
 * integers. */
#define VALUE(v) (void *)(ptrdiff_t)(v)

#define ZEROFY(p, n) memset((p), 0, sizeof(*p) * (n))

#define COPY(src, dest) memcpy((dest), (src), sizeof(*(src)))

#endif
