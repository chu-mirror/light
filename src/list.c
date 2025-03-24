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
	push(arr + (len-i-1)*size, &l);
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

List find_first(int jdg(void *), List l) {
    while (!is_empty_list(l)) {
	if (jdg(car(l))) {
	    return l;
	}
	l = cdr(l);
    }
    return empty_list;
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

List push(void *elm, List *l_r) {
    return *l_r = cons(elm, *l_r);
}

void *pop(List *l_r) {
    List _l = cdr(*l_r);
    void *v = car(*l_r);
    (*l_r)->rest = empty_list;
    FREE(*l_r);
    *l_r = _l;
    return v;
}

void remove_all(int jdg(void *), List *l_r) {
    List tail, next;

    tail = empty_list;
    next = *l_r;
    while (!is_empty_list(next)) {
	if (jdg(car(next))) {
	    pop(&next);
	    if (is_empty_list(tail)) {
		*l_r = next;
            } else {
		tail->rest = next;
	    }
        } else {
	    tail = next;
	    next = cdr(next);
	}
    }
}

void *remove_first(int jdg(void *), List *l_r) {
    List l;
    l = find_first(jdg, *l_r);
    if (is_empty_list(l)) {
	return NULL;
    }
    if (l == *l_r) {
	return pop(l_r);
    }
    do {
	List _l = *l_r;
        while (cdr(_l) != l)
	    ;
	_l->rest = l->rest;
        return pop(&l);
    } while (0);
}
