#include <string.h>

#include "light.h"
#include "atom.h"
#include "hash_table.h"

#ifndef LIGHT_ATOM_HASH_TABLE_SIZE
#define LIGHT_ATOM_HASH_TABLE_SIZE 128
#endif

struct atom {
    const uint8_t *block;
    size_t len;
};

static HashTable atoms;

static uint32_t
hash_func_atom(const void *a)
{
    struct atom *_a;
    RENAME(a, _a);

    return hash(_a->block, _a->len) % LIGHT_ATOM_HASH_TABLE_SIZE;
}

static int
equal_func_atom(const void *a1, const void *a2)
{
    struct atom *_a1, *_a2;
    RENAME(a1, _a1);
    RENAME(a2, _a2);
    return _a1->len == _a2->len
           && memcmp(_a1->block, _a2->block, _a1->len) == 0;
}

void *
atom(const uint8_t *blk, size_t len)
{
    struct atom a;
    void *p;
    if (atoms == NULL) {
        RESERVE(new_hash_table(&atoms, hash_func_atom, equal_func_atom));
    }

    a.block = blk;
    a.len = len;

    p = get_from_hash_table(atoms, &a);

    if (p == NULL) {
        uint8_t *_blk = NULL;
        RESERVE(MOVE_ARRAY(blk, _blk, len));
        a.block = _blk;

        struct atom *a_r = NULL;
        RESERVE(MOVE(&a, a_r); put_to_hash_table(atoms, a_r, a_r););
        return a_r;
    }

    return p;
}

const char *
atom_str(const char *str)
{
    struct atom *a_r;
    RENAME(atom((const uint8_t *)str, strlen(str) + 1), a_r);
    return (const char *)(a_r->block);
}
