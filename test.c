#include <stdio.h>

#include "light.h"
#include "list.h"

void inc(void *n) {
    (*(int *)n)++;
}

int main() {
    char (*hello)[6];
    NEW0(hello);
    (*hello)[0] = 'H';
    (*hello)[1] = 'e';
    (*hello)[2] = 'l';
    (*hello)[3] = 'l';
    (*hello)[4] = 'o';
    (*hello)[5] = '\0';
    printf("%s's hash is %08X\n", *hello, hash_str(*hello));
    FREE(hello);

    int arr[3] = {1, 2, 3}, arr_[3];
    List l1 = empty_list, l2;
    l1 = cons(&arr[2], l1);
    l1 = cons(&arr[1], l1);
    l1 = cons(&arr[0], l1);

    do {
	List l;
	for (l = l1; !is_empty_list(l); l = cdr(l)) {
	    printf("%d", *(int *)car(l));
	}
	printf("\n");
    } while (0);

    do {
	List l;
	map(inc, l1);
	for (l = l1; !is_empty_list(l); l = cdr(l)) {
	    printf("%d", *(int *)car(l));
	}
	printf("\n");
    } while (0);

    free_list(&l1);
    free_list(&l2);
    
    return 0;
}
