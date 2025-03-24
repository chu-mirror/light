#include <stdio.h>
#include <assert.h>

#include "light.h"
#include "list.h"

void inc(void *n) {
    (*(int *)n)++;
}

int main() {
    printf("Testing version %d.%02d\n", LIGHT_VERSION / 100, LIGHT_VERSION % 100);
    do {
	char (*hello)[6] = NULL;
	NEW0(hello);
	assert(hello != NULL);
	(*hello)[0] = 'H';
	(*hello)[1] = 'e';
	(*hello)[2] = 'l';
	(*hello)[3] = 'l';
	(*hello)[4] = 'o';
	(*hello)[5] = '\0';
	FREE(hello);
	assert(hello == NULL);
    } while (0);

    do {
	char str[] = "HASHED STRING";
	assert(hash_str(str) == 0xCD0CDDA4);
    } while (0);

    do {
	int arr[6] = {1, 2, 3, 4, 5, 6};
        List l1 = empty_list, l2 = empty_list, l3;

	do {
	    l1 = LIST(arr, 3);
	    assert(length(l1) == 3);

	    l2 = LIST(arr+3, 3);
	    assert(length(l2) == 3);

	    l3 = append(&l1, &l2);
	    assert(length(l3) == 6);
	} while (0);

	do {
	    int i, *np;

	    i = 1;
            FOREACH(np, l3) {
		assert(*np == i);
		++i;
	    }
	} while (0);

	do {
	    int i, *np;

	    map(inc, l3);

	    i = 1;
            FOREACH(np, l3) {
		assert(*np == i+1);
		++i;
	    }
	} while (0);

	free_list(&l3);
    } while (0);

    printf("Passed all tests\n");
    
    return 0;
}
