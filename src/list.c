#include "list.h"
#include "memory.h"

struct list {
    struct list *rest;
    void *first;
};

List cons(void *ele, List l) {
    List _l;

    NEW(_l);
    _l->first = ele;
    _l->rest = l;

    return _l;
}

void *car(List l) {
    return l->first;
}

List cdr(List l) {
    return l->rest;
}

List list_from_arr(void *arr, size_t len, size_t size) {
    int i;
    List l = empty_list;
    for (i = 0; i < len; ++i) {
	l = cons(arr + (len-i-1)*size, l);
    }
    return l;
}

size_t length(List l) {
    size_t len;

    for (len = 0; !is_empty_list(l); l = cdr(l)) {
	++len;
    }

    return len;
}

void map(void f(void *), List l) {
    for (; !is_empty_list(l); l = cdr(l)) {
	f(car(l));
    }
}

List append(List *l1_r, List *l2_r) {
    List l;

    if (is_empty_list(*l1_r)) {
	l = *l2_r;
	*l2_r = empty_list;
	return l;
    }

    do { /* find tail |l| of |*l1_r| */
	l = *l1_r;
	while (!is_empty_list(cdr(l))) {
	    l = cdr(l);
	}
	l->rest = *l2_r;
    } while(0);

    l = *l1_r;

    *l1_r = empty_list;
    *l2_r = empty_list;

    return l;
}

void free_list(List *l_r) {
    List l = *l_r;

    while (!is_empty_list(l)) {
	List _l = l;
	l = cdr(l);
	FREE(_l);
    }

    *l_r = empty_list;
}


