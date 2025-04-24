#include <assert.h>

#include "light.h"
#include "list.h"
#include "table_common.h"
#include "hash_table.h"

struct hash_table {
    size_t size;
    List *table;
    List slots;
    uint32_t (*hash_func)(const void *);
    int (*equal_func)(const void *, const void *);
};

static inline List *record_list(HashTable tbl, const void *key) {
    uint32_t hv;
    hv = CLOSURE(tbl->hash_func)(key);
    return &tbl->table[hv % tbl->size];
}

struct record_key_equal_to_the_key_FRAME {
    HashTable table;
    const void *key;
};

static int record_key_equal_to_the_key_FUNC(void *rcd) {
    struct record_key_equal_to_the_key_FRAME *fr;
    INIT_CLOSURE_FRAME(fr);

    return CLOSURE(fr->table->equal_func)(fr->key, ((TableRecord *)rcd)->k);
}

struct equal_to_the_records_reference_FRAME {
    List *records_reference;
};

static int equal_to_the_records_reference_FUNC(void *rcds_r) {
    struct equal_to_the_records_reference_FRAME *fr;
    INIT_CLOSURE_FRAME(fr);

    return rcds_r == fr->records_reference;
}

HashTable new_hash_table(size_t n, uint32_t hf(const void *),
                     int eq(const void *, const void *)) {
    HashTable tbl;

    NEW(tbl);
    do {
	int p = 0;
	while (n > 1 << p) ++p;
	tbl->size = 1 << p;
    } while (0);
    CALLOC(tbl->table, tbl->size);
    tbl->slots = empty_list;
    tbl->hash_func = hf;
    tbl->equal_func = eq;
    
    return tbl;
}

void free_hash_table(HashTable *tbl_r) {
    List *rcds_r; /* records */

    FOREACH(rcds_r, (*tbl_r)->slots) {
	TableRecord *rcd_r;
        FOREACH(rcd_r, *rcds_r) {
	    FREE(rcd_r);
	}
	free_list(rcds_r);
    }
    free_list(&(*tbl_r)->slots);
    FREE((*tbl_r)->table);
    FREE(*tbl_r);
}

void put_to_hash_table(HashTable tbl, const void *key, void *v) {
    List *rcds_r; /* records */

    rcds_r = record_list(tbl, key);

    do { /* search the already stored keys */
	List res;
	Closure cl;
	struct record_key_equal_to_the_key_FRAME fr = {tbl, key};

	NEW_CLOSURE(cl, record_key_equal_to_the_key_FUNC, &fr);
	res = find_first((void *) cl, *rcds_r);

	if (!is_empty_list(res)) {
	    ((TableRecord *)car(res))->v = v;
	    return;
	}
    } while (0);

    do { /* create a new record */
	TableRecord *nrcd_r; /* new record */
	do { /* initialize the new record */
	    NEW(nrcd_r);
	    nrcd_r->k = key;
	    nrcd_r->v = v;
	} while (0);

        if (is_empty_list(*rcds_r)) {
	    push(rcds_r, &tbl->slots);
	}

	push(nrcd_r, rcds_r);
    } while (0);
}

void *get_from_hash_table(HashTable tbl, const void *key) {
    List *rcds_r; /* records */

    rcds_r = record_list(tbl, key);

    do {
	List res;
	Closure cl;
	struct record_key_equal_to_the_key_FRAME fr = {tbl, key};

	NEW_CLOSURE(cl, record_key_equal_to_the_key_FUNC, &fr);
	res = find_first((void *) cl, *rcds_r);

	if (!is_empty_list(res)) {
	    return ((TableRecord *)car(res))->v;
	}
    } while (0);
    return NULL;
}

void remove_from_hash_table(HashTable tbl, const void *key) {
    List *rcds_r; /* records */

    rcds_r = record_list(tbl, key);

    do {
	TableRecord *rcd_r;
	Closure cl;
	struct record_key_equal_to_the_key_FRAME fr = {tbl, key};

	NEW_CLOSURE(cl, record_key_equal_to_the_key_FUNC, &fr);
	rcd_r = (TableRecord *) remove_first((void *)cl, rcds_r);

	FREE(rcd_r);
    } while (0);

    if (is_empty_list(*rcds_r)) {
	Closure cl;
	struct equal_to_the_records_reference_FRAME fr = {rcds_r};

	NEW_CLOSURE(cl, equal_to_the_records_reference_FUNC, &fr);
	remove_first((void *)cl, &tbl->slots);
    }
}
