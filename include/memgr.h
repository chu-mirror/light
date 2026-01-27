#ifndef LIGHT_MEMGR_H
#define LIGHT_MEMGR_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

#ifndef NDEBUG
#include <stdio.h>
#endif

extern size_t _light_alloc_count;
extern size_t _light_reserved_count;

struct light_reserving_frame {
    struct light_reserving_frame *prev;
    size_t start_alloc_count;
    size_t doubled_count;
};
extern struct light_reserving_frame *_light_current_reserving_frame;

#define RESERVE(...)                                                          \
    do {                                                                      \
        struct light_reserving_frame *fr_r;                                   \
        size_t _actual_reserved_count;                                        \
        fr_r = (struct light_reserving_frame *)malloc(sizeof(*fr_r));         \
        fr_r->prev = _light_current_reserving_frame;                          \
        fr_r->start_alloc_count = _light_alloc_count;                         \
        fr_r->doubled_count = 0;                                              \
        _light_current_reserving_frame = fr_r;                                \
        do {                                                                  \
            __VA_ARGS__;                                                      \
        } while (0);                                                          \
        _actual_reserved_count = _light_alloc_count - fr_r->start_alloc_count \
                                 - fr_r->doubled_count;                       \
        _light_reserved_count += _actual_reserved_count;                      \
        if (fr_r->prev) {                                                     \
            fr_r->prev->doubled_count +=                                      \
                _light_alloc_count - fr_r->start_alloc_count;                 \
        }                                                                     \
        _light_current_reserving_frame = fr_r->prev;                          \
        free(fr_r);                                                           \
    } while (0)

#define KEEP(...)                                                   \
    do {                                                            \
        size_t diff = _light_alloc_count - _light_reserved_count;   \
        do {                                                        \
            __VA_ARGS__;                                            \
        } while (0);                                                \
        assert(diff == _light_alloc_count - _light_reserved_count); \
    } while (0)

#define assert_memory_safety()                          \
    assert(_light_alloc_count == _light_reserved_count)

#define NEW(p)                             \
    (++_light_alloc_count,                 \
     assert((p) == NULL),                  \
     (p) = (typeof(p))malloc(sizeof *(p)))

#define CALLOC(p, len)                            \
    (++_light_alloc_count,                        \
     assert((p) == NULL),                         \
     (p) = (typeof(p))calloc((len), sizeof *(p)))

#define NEW0(p) CALLOC((p), 1)

#define FREE(p)                                                              \
    ((void)(assert((p) != NULL), --_light_alloc_count, free(p), (p) = NULL))

#define REALLOC_ARRAY(p, len)                              \
    ((p) = (typeof(p))reallocarray((p), sizeof *(p), len))

#define STRDUP(src, dest)            \
    ((void)(assert((dest) == NULL)), \
     ++_light_alloc_count,           \
     (dest) = strdup(src))
#define MOVE(src, dest) (NEW(dest), memcpy((dest), (src), sizeof *(dest)))
#define MOVE_ARRAY(src, dest, n)                                   \
    (CALLOC(dest, n), memcpy((dest), (src), sizeof *(dest) * (n)))

#ifndef NDEBUG
static inline void
print_memgr_count(const char *cond)
{
    fprintf(
        stderr,
        "condition: %s; alloc: %d; reserved: %d\n",
        cond,
        _light_alloc_count,
        _light_reserved_count
    );
}
#else
#define print_memgr_count()
#endif

#endif
