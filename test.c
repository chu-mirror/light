#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "light.h"
#include "list.h"
#include "hash_table.h"

struct inc1_frame {
    int n;
};

void inc1(void) {
    struct inc1_frame *fr;

    INIT_CLOSURE_FRAME(fr);
    ++fr->n;
}

void do_inc1(void *cl(void))
{
    CLOSURE(cl)();
}

void inc(void *n) {
    (*(int *)n)++;
}

int less_than_4(void *n) {
    return *(int *)n < 4;
}

uint32_t hash_func_str(const void *str) {
    return hash_str((const char *)str);
}

int equal_func_str(const void *str1, const void *str2) {
    return strcmp((const char *)str1, (const char *)str2) == 0;
}

int main() {
    printf("Testing version %d.%02d\n", LIGHT_VERSION / 100, LIGHT_VERSION % 100);
    do {
	char (*hello)[6] = NULL;
	NEW0(hello);
	assert(hello != NULL);
	strcpy(*hello, "Hello");
	FREE(hello);
	assert(hello == NULL);
    } while (0);

    assert(alloc_count == 0);

    do {
	char str[] = "HASHED STRING";
	assert(hash_str(str) == 0xCD0CDDA4);
    } while (0);

    do {
	struct inc1_frame *fr;
	Closure cl;
	NEW0(fr);
	assert(fr->n == 0);

	NEW(cl);
	init_closure(cl, inc1, fr);

	do_inc1((void *)cl);
	assert(fr->n == 1);

	do_inc1((void *)cl);
	assert(fr->n == 2);

	FREE(fr);
	FREE(cl);
    } while (0);

    do {
	int arr[6] = {1, 2, 3, 4, 5, 6};
        List l1 = empty_list, l2 = empty_list, l3;

	do { /* test |append| */
	    l1 = _LIST(arr, 3);
	    assert(length(l1) == 3);

	    l2 = _LIST(arr+3, 3);
	    assert(length(l2) == 3);

	    l3 = append(&l1, &l2);
	    assert(length(l3) == 6);

	    do { /* ensure that |l3| has correct elements */
		int i, *np;

		i = 0;
		FOREACH(np, l3) {
		    assert(*np == i+1);
		    ++i;
		}
	    } while (0);
	} while (0);

	do { /* test |map| */
	    int i, *np;

	    map(inc, l3);

	    i = 0;
            FOREACH(np, l3) {
		assert(*np == i+2);
		assert(arr[i] == i+2);
		++i;
	    }
	} while (0);

        do { /* test |push| and |pop| */
	    int n;
	    n = *(int *)pop(&l3);
            assert(n == 2);
	    assert(length(l3) == 5);
            push(&arr[0], &l3);
	    assert(length(l3) == 6);
	} while (0);

        do { /* test |remove_all| */
	    int i, *np;
	    remove_all(less_than_4, &l3);
            assert(length(l3) == 4);

	    i = 0;
            FOREACH(np, l3) {
		assert(*np == i+4);
		++i;
	    }
	} while (0);

        do { /* test |remove_first| */
	    int i, *np;
	    push(&arr[0], &l3);
	    assert(remove_first(less_than_4, &l3) == &arr[0]);
            assert(length(l3) == 4);

	    i = 0;
            FOREACH(np, l3) {
		assert(*np == i+4);
		++i;
	    }
	} while (0);

	free_list(&l3);

	assert(is_empty_list(l1));
	assert(is_empty_list(l2));
	assert(is_empty_list(l3));
    } while (0);

    assert(alloc_count == 0);

    do {
	HashTable tbl;
	tbl = new_hash_table(5, hash_func_str, equal_func_str);
	assert(alloc_count == 2);

	put_to_hash_table(tbl, "Chu", "Hello");
	assert(alloc_count == 5);

	put_to_hash_table(tbl, "Mzz", "Welcome");
	assert(alloc_count == 8);

	assert(strcmp((char *)get_from_hash_table(tbl, "Chu"), "Hello") == 0);
	assert(strcmp((char *)get_from_hash_table(tbl, "Mzz"), "Welcome") == 0);

	put_to_hash_table(tbl, "Chu", "Welcome");
	assert(strcmp((char *)get_from_hash_table(tbl, "Chu"), "Welcome") == 0);
	assert(alloc_count == 8);

	remove_from_hash_table(tbl, "Chu");
	assert(get_from_hash_table(tbl, "Chu") == NULL);
	assert(alloc_count == 5);
	free_hash_table(&tbl);
    } while (0);

    assert(alloc_count == 0);

    printf("Passed all tests\n");
    return 0;
}
