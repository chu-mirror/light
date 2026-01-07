#ifndef LIGHT_MEMGR_H
#define LIGHT_MEMGR_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

extern size_t _light_alloc_count;
extern size_t _light_reserved_count;
extern bool _light_reserving;

#define RESERVE(stat)                                                \
    do {                                                             \
        size_t _alloc_count = _light_alloc_count;                    \
        bool _prev_reserving = _light_reserving;                     \
        _light_reserving = true;                                     \
        stat;                                                        \
        _light_reserved_count +=                                     \
            _prev_reserving ? 0 : _light_alloc_count - _alloc_count; \
        _light_reserving = _prev_reserving;                          \
    } while (0)
#define KEEP(stat)                                                  \
    do {                                                            \
        size_t diff = _light_alloc_count - _light_reserved_count;   \
        stat;                                                       \
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

#define MOVE(src, dest) (NEW(dest), memcpy((dest), (src), sizeof *(src)))
#define MOVE_ARRAY(src, dest, n)                                  \
    (CALLOC(dest, n), memcpy((dest), (src), sizeof *(src) * (n)))

#endif
