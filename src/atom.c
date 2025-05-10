#include <string.h>

#include "light.h"
#include "atom.h"
#include "hash_table.h"

struct atom {
    uint8_t *block;
    size_t len;
};

static HashTable atoms;

static uint32_t
hash_func_atom(const void *a)
{
    struct atom *_a;
    RENAME(a, _a);

    return hash(_a->block, _a->len);
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
    uint8_t *_blk;
    void *p;
    if (atoms == NULL) {
        atoms = new_hash_table(100, hash_func_atom, equal_func_atom);
    }

    MOVE_ARRAY(blk, _blk, len);
    a.block = _blk;
    a.len = len;

    p = get_from_hash_table(atoms, &a);

    if (p == NULL) {
        struct atom *a_r;
        MOVE(&a, a_r);
        put_to_hash_table(atoms, a_r, a_r);
        return a_r;
    }
    FREE(_blk);
    return p;
}

void
free_atom(const uint8_t *blk, size_t len)
{
    struct atom *a_r, a;
    uint8_t *_blk;

    if (atoms == NULL) {
        return;
    }

    MOVE_ARRAY(blk, _blk, len);
    a.block = _blk;
    a.len = len;

    a_r = (struct atom *)remove_from_hash_table(atoms, &a);
    if (a_r != NULL) {
        FREE(a_r->block);
        FREE(a_r);
    }
    FREE(_blk);
}

void
clear_atoms()
{
    TableRecord *rcd_r;
    List rcd_r_s;
    rcd_r_s = get_all_records_from_hash_table(atoms);
    FOREACH (rcd_r, rcd_r_s) {
        struct atom *a_r;
        RENAME(rcd_r->v, a_r);
        FREE(a_r->block);
        FREE(a_r);
    }
    free_hash_table(&atoms);
    free_list(&rcd_r_s);
}

void *
atom_str(const char *str)
{
    return atom((const uint8_t *)str, strlen(str));
}

void
free_atom_str(const char *str)
{
    free_atom((const uint8_t *)str, strlen(str));
}
