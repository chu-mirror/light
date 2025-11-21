/* #define NDEBUG */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "light.h"
#include "list.h"
#include "hash_table.h"
#include "atom.h"
#include "assoc_table.h"
#include "array.h"
#include "deque.h"
#include "str.h"

struct inc1_frame {
    int n;
};

void
inc1(void)
{
    struct inc1_frame *fr;

    INIT_CLOSURE_FRAME(fr);
    ++fr->n;
}

void
do_inc1(void *cl(void))
{
    CLOSURE(cl)();
}

void
inc(void *n)
{
    (*(int *)n)++;
}

int
less_than_4(void *n)
{
    return *(int *)n < 4;
}

static int
equal_func_str(const void *s1, const void *s2)
{
    return strcmp((const char *)s1, (const char *)s2) == 0;
}

int
main()
{
    printf(
        "Testing version %d.%02d\n", LIGHT_VERSION / 100, LIGHT_VERSION % 100
    );
    do {
        char(*hello)[6] = NULL;
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

    do { /* basic usage of closure */
        struct inc1_frame *fr;
        Closure cl;
        NEW0(fr);
        assert(fr->n == 0);

        NEW_CLOSURE(cl, inc1, fr);

        do_inc1((void *)cl);
        assert(fr->n == 1);

        do_inc1((void *)cl);
        assert(fr->n == 2);

        FREE_CLOSURE(cl);
        FREE(fr);
    } while (0);
    assert(closure_count == 0);
    assert(alloc_count == 0);

    do { /* allocation of closures */
#define NUMBER_OF_CLOSURES 3000
        int i;
        struct inc1_frame *fr;
        Closure cls[NUMBER_OF_CLOSURES];

        NEW0(fr);
        assert(fr->n == 0);

        for (i = 0; i < NUMBER_OF_CLOSURES; ++i) {
            assert(closure_count == i);
            assert(closure_count == number_of_closures());
            assert(fr->n == i);

            NEW_CLOSURE(cls[i], inc1, fr);
            do_inc1((void *)cls[i]);
        }

        for (i = 0; i < NUMBER_OF_CLOSURES; ++i) {
            assert(closure_count == NUMBER_OF_CLOSURES - i);
            assert(closure_count == number_of_closures());

            FREE_CLOSURE(cls[i]);
        }

        FREE(fr);
#undef NUMBER_OF_CLOSURES
    } while (0);
    assert(closure_count == 0);
    assert(closure_count == number_of_closures());
    assert(alloc_count == 0);

    do {
        int arr[6] = {1, 2, 3, 4, 5, 6};
        List l1 = empty_list, l2 = empty_list, l3, l4;

	do { /* test |reverse| */
	    l1 = _LIST(arr, 6);
	    reverse(&l1);
	    
            do {
                int i, *np;

                i = 0;
                FOREACH (np, l1) {
                    assert(*np == 6-i);
                    ++i;
                }
            } while (0);
	    free_list(&l1);
	} while (0);

        do { /* test |append| */
            l1 = _LIST(arr, 3);
            assert(length(l1) == 3);

            l2 = _LIST(arr + 3, 3);
            assert(length(l2) == 3);

            l3 = append(&l1, &l2);
            assert(length(l3) == 6);

            do { /* ensure that |l3| has correct elements */
                int i, *np;

                i = 0;
                FOREACH (np, l3) {
                    assert(*np == i + 1);
                    ++i;
                }
            } while (0);
        } while (0);

        do { /* test |copy_list| */
            int *elm;
            l4 = copy_list(l3);
            assert(length(l3) == length(l4));

            List _l3 = l3;
            FOREACH (elm, l4) {
                assert(elm == car(_l3));
                _l3 = cdr(_l3);
            }
        } while (0);

        do { /* test |map| */
            int i, *np;

            map(inc, l3);

            i = 0;
            FOREACH (np, l3) {
                assert(*np == i + 2);
                assert(arr[i] == i + 2);
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
            FOREACH (np, l3) {
                assert(*np == i + 4);
                ++i;
            }
        } while (0);

        do { /* test |remove_first| */
            int i, *np;
            push(&arr[0], &l3);
            assert(remove_first(less_than_4, &l3) == &arr[0]);
            assert(length(l3) == 4);

            i = 0;
            FOREACH (np, l3) {
                assert(*np == i + 4);
                ++i;
            }
        } while (0);

        free_list(&l3);
        assert(alloc_count == 6);
        free_list(&l4);

        assert(is_empty_list(l1));
        assert(is_empty_list(l2));
        assert(is_empty_list(l3));
    } while (0);

    assert(alloc_count == 0);

    do {
        HashTable tbl;
        tbl = new_string_hash_table();
        assert(alloc_count == 3);

        put_to_hash_table(tbl, "Chu", "Hello");
        assert(alloc_count == 6);

        put_to_hash_table(tbl, "Mzz", "Welcome");
        assert(alloc_count == 9);

        assert(strcmp((char *)get_from_hash_table(tbl, "Chu"), "Hello") == 0);
        assert(
            strcmp((char *)get_from_hash_table(tbl, "Mzz"), "Welcome") == 0
        );

        put_to_hash_table(tbl, "Chu", "Welcome");
        assert(
            strcmp((char *)get_from_hash_table(tbl, "Chu"), "Welcome") == 0
        );
        assert(alloc_count == 9);

        remove_from_hash_table(tbl, "Chu");
        assert(get_from_hash_table(tbl, "Chu") == NULL);
        assert(alloc_count == 6);
        free_hash_table(&tbl);
    } while (0);

    assert(alloc_count == 0);

    do {
        AssocTable tbl;
        tbl = new_assoc_table(equal_func_str);
        assert(alloc_count == 1);

        put_to_assoc_table(tbl, "Chu", "Hello");
        assert(alloc_count == 3);

        put_to_assoc_table(tbl, "Mzz", "Welcome");
        assert(alloc_count == 5);

        assert(strcmp((char *)get_from_assoc_table(tbl, "Chu"), "Hello") == 0);
        assert(
            strcmp((char *)get_from_assoc_table(tbl, "Mzz"), "Welcome") == 0
        );

        put_to_assoc_table(tbl, "Chu", "Welcome");
        assert(
            strcmp((char *)get_from_assoc_table(tbl, "Chu"), "Welcome") == 0
        );
        assert(alloc_count == 5);

        remove_from_assoc_table(tbl, "Chu");
        assert(get_from_assoc_table(tbl, "Chu") == NULL);
        assert(alloc_count == 3);
        free_assoc_table(&tbl);
    } while (0);

    do {
        char *str1, *str2;
        CALLOC(str1, 10);
        CALLOC(str2, 10);
        strcpy(str1, "Hello");
        strcpy(str2, "Hello");
        assert(str1 != str2);
        assert(atom_str(str1) == atom_str(str2));
        do {
            size_t c;
            free_atom_str(str1);
            c = alloc_count;
            free_atom_str(str2);
            assert(c == alloc_count);

            atom_str(str1);
            c = alloc_count;
            atom_str(str2);
            assert(c == alloc_count);
        } while (0);
        FREE(str1);
        FREE(str2);
        clear_atoms();
    } while (0);

    do {
        Array arr;
        int *n;
        arr = new_array();

        *nth(arr, 0) = VALUE((int8_t)-1);
        RENAME(nth(arr, 0), n);
        assert(*nth(arr, 0) == VALUE(-1));

        *nth(arr, 1) = VALUE(1);
        RENAME(nth(arr, 1), n);
        ++(*n);
        assert(*nth(arr, 1) == VALUE(2));

        *nth(arr, 2000) = VALUE(1);
        RENAME(nth(arr, 2000), n);
        ++(*n);
        assert(*nth(arr, 2000) == VALUE(2));

        for (int i = 2; i < 2000; ++i) {
            assert(*nth(arr, i) == VALUE(0));
        }

        free_array(&arr);
    } while (0);

    do {
        Deque q;
        void *elm;

        q = new_deque();
        assert(is_empty_deque(q));

        push_back(q, VALUE(1));
        assert(*nth_of_deque(q, 0) == VALUE(1));
        assert(!is_empty_deque(q));

        elm = pop_back(q);
        assert(elm == VALUE(1));
        assert(is_empty_deque(q));

        int i;
        for (i = 0; i < 100; ++i) {
            push_back(q, VALUE(i));
        }
        for (i = 0; i < 100; ++i) {
            push_front(q, VALUE(i));
        }
        for (i = 0; i < 100; ++i) {
            elm = pop_front(q);
            assert(elm == VALUE(99 - i));
        }
        for (i = 0; i < 100; ++i) {
            elm = pop_front(q);
            assert(elm == VALUE(i));
        }
        assert(is_empty_deque(q));

        free_deque(&q);
        assert(q == NULL);
    } while (0);

    do {
	Str str;
	str = new_str("");
	for (int i = 0; i < 10; ++i) {
	    static char num[2];
	    num[0] = '0';
	    num[0] += i;
	    str_extend(str, num);
	}
	assert(strcmp(raw_string(str), "0123456789") == 0);
	assert(str_length(str) == 10);
	free_str(&str);

	str = new_str("");
	for (int i = 0; i < 10; ++i) {
	    str_extend(str, "Hello");
	}
	assert(str_length(str) == 10 * strlen("Hello"));
	free_str(&str);
    } while (0);

    assert(alloc_count == 0);
    assert(closure_count == 0);

    printf("Passed all tests\n");
    return 0;
}
