#include <assert.h>

#include "light.h"
#include "table_common.h"
#include "assoc_table.h"
#include "list.h"

struct assoc_table {
    List records;
    int (*equal_func)(const void *, const void *);
};

struct record_key_equal_to_the_key_FRAME {
    AssocTable tbl;
    const void *key;
};

static int record_key_equal_to_the_key_FUNC(void *rcd) {
    struct record_key_equal_to_the_key_FRAME *fr;
    INIT_CLOSURE_FRAME(fr);

    return CLOSURE(fr->tbl->equal_func)(fr->key, ((TableRecord *)rcd)->k);
}

AssocTable new_assoc_table(int eq(const void *, const void *))
{
    AssocTable tbl;
    NEW(tbl);

    tbl->records = empty_list;
    tbl->equal_func = eq;

    return tbl;
}

void free_assoc_table(AssocTable *tbl_r)
{
    do {
	TableRecord *rcd_r;
        FOREACH(rcd_r, (*tbl_r)->records) {
	    FREE(rcd_r);
	}
    } while (0);

    free_list(&(*tbl_r)->records);
    FREE(*tbl_r);
}

void put_to_assoc_table(AssocTable tbl, const void *key, void *v)
{
    do { /* if |key| is already in |tbl|, set to |v| and return */
	Closure cl;
	List res;
	struct record_key_equal_to_the_key_FRAME fr = {tbl, key};
	
	NEW_CLOSURE(cl, record_key_equal_to_the_key_FUNC, &fr);
	res = find_first((void *)cl, tbl->records);
	FREE(cl);

	if (!is_empty_list(res)) {
	    ((TableRecord *)car(res))->v = v;
	    return;
	}
    } while (0);

    do { /* Create new record */
	TableRecord *nrcd_r;
	NEW(nrcd_r);
	nrcd_r->k = key;
	nrcd_r->v = v;
	push(nrcd_r, &tbl->records);
    } while (0);
}

void *get_from_assoc_table(AssocTable tbl, const void *key)
{
    Closure cl;
    List res;
    struct record_key_equal_to_the_key_FRAME fr = {tbl, key};

    NEW_CLOSURE(cl, record_key_equal_to_the_key_FUNC, &fr);
    res = find_first((void *)cl, tbl->records);
    FREE(cl);

    if (!is_empty_list(res)) {
	return ((TableRecord *)car(res))->v;
    }
    return NULL;
}

void remove_from_assoc_table(AssocTable tbl, const void *key)
{
    Closure cl;
    TableRecord *rcd_r;
    struct record_key_equal_to_the_key_FRAME fr = {tbl, key};

    NEW_CLOSURE(cl, record_key_equal_to_the_key_FUNC, &fr);
    RENAME(remove_first((void *)cl, &tbl->records), rcd_r);
    FREE(rcd_r);
    FREE(cl);
}
