#include "light.h"
#include "closure.h"

#include "stdint.h"
#include "string.h"

struct closure {
    void *func;
    void *frame;
};

void *_light_closure_frame;
size_t closure_count;

#define CLOSURE_POOL_INIT_SIZE (1 << 6)

struct closure_pool {
    struct closure *closures;
    size_t size;
    uint8_t *label;
    Closure bases[64]; /* To ensure the size of pool can double (64-1) times.
                        * It's a safe assumption... Right?
                        * It will lead to severe performance issue if that much
                        * space was used though.
                        */
    int next_base;
} closure_pool;

#define closure_pool_current_base                  \
    closure_pool.bases[closure_pool.next_base - 1]

static inline int
root_of(int l, int r)
{
    return (r - l) / 2 + l - 1;
}

static inline int
position_of(Closure cl)
{
    int p;

    for (p = closure_pool.next_base; p > 0; --p) {
        if (cl >= closure_pool.bases[p - 1]
            && cl < closure_pool.bases[p - 1]
                        + (CLOSURE_POOL_INIT_SIZE << (p - 1))) {
            return cl - closure_pool.bases[p - 1];
        }
    }
    return -1;
}

size_t
number_of_closures()
{
    size_t n = 0;

    int i;
    for (i = 0; i < closure_pool.size << 1; i += 2) {
        n += closure_pool.label[i];
    }

    return n;
}

void
init_closure(Closure cl, void *func, void *frame)
{
    cl->func = func;
    cl->frame = frame;
}

void *
closure(Closure cl)
{
    int p = position_of(cl);

    if (p == -1) {
        return cl;
    }

    _light_closure_frame = closure_pool.closures[p].frame;
    return closure_pool.closures[p].func;
}

SENTENCE
find_an_empty_slot_as_ncl(Closure *ncl_r)
{
    int l, r;

    l = 0;
    r = closure_pool.size << 1;

    while (r - l > 2) {
        int mid = l + (r - l) / 2;

        if (closure_pool.label[root_of(l, mid)] == 0) {
            r = mid;
        } else {
            l = mid;
        }
    }
    do { /* re label the path */
        int p, lv;

        closure_pool.label[l] = 1;

        p = l;
        lv = 1;
        do {
            ++lv;
            p = root_of(p >> lv << lv, (p >> lv << lv) + (1 << lv));
            closure_pool.label[p] =
                closure_pool.label[root_of(p + 1 - (1 << (lv - 1)), p + 1)]
                && closure_pool.label[root_of(p + 1, p + 1 + (1 << (lv - 1)))];
        } while (1 << lv != closure_pool.size && closure_pool.label[p] != 0);
    } while (0);
    *ncl_r = &closure_pool.closures[l >> 1];
}

Closure
alloc_closure(void)
{
    Closure ncl; /* new closure */
    do { /* allocate memory if not enough */
        if (closure_pool.size == 0) {
            CALLOC_(closure_pool.closures, CLOSURE_POOL_INIT_SIZE);
            CALLOC_(closure_pool.label, CLOSURE_POOL_INIT_SIZE << 1);
            closure_pool.size = CLOSURE_POOL_INIT_SIZE;
            closure_pool.bases[closure_pool.next_base++] =
                closure_pool.closures;
        }
        if (closure_pool.label[root_of(0, closure_pool.size)]) {
            /* Do not free memory here to keep the space from being reused for
             * other purposes. */
            closure_pool.size <<= 1;
            CALLOC_(closure_pool.closures, closure_pool.size);
            closure_pool.bases[closure_pool.next_base++] =
                closure_pool.closures;

            REALLOC_ARRAY(closure_pool.label, closure_pool.size << 1);
            memset(
                closure_pool.label + closure_pool.size, 0, closure_pool.size
            );
        }
    } while (0);

    find_an_empty_slot_as_ncl(&ncl);
    return ncl;
}

void
free_closure(Closure cl)
{
    int p, lv;

    p = position_of(cl) * 2;
    lv = 1;
    closure_pool.label[p] = 0;

    do {
        ++lv;
        p = root_of(p >> lv << lv, (p >> lv << lv) + (1 << lv));
        if (closure_pool.label[p] == 0) {
            break;
        }
        closure_pool.label[p] = 0;
    } while (1 << lv != closure_pool.size);
}
