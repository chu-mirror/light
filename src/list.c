#include "light.h"
#include "list.h"

struct list {
    struct list *rest;
    void *first;
};

List
cons(void *ele, List l)
{
    List _l = NULL;

    NEW(_l);
    _l->first = ele;
    _l->rest = l;

    return _l;
}

void *
car(List l)
{
    return l->first;
}

List
cdr(List l)
{
    return l->rest;
}

List
list_from_arr(void *arr, size_t len, size_t size)
{
    int i;
    List l = empty_list;
    for (i = 0; i < len; ++i) {
        push(arr + (len - i - 1) * size, &l);
    }
    return l;
}

List
copy_list(List l)
{
    List _l = empty_list;

    do {
        List tail, __l;
        void *elm;

        tail = cons(NULL, empty_list);
        __l = tail;
        FOREACH (elm, l) {
            List nn; /* new node */
            nn = cons(elm, empty_list);
            tail->rest = nn;
            tail = nn;
        }
        _l = cdr(__l);
        FREE(__l);
    } while (0);

    return _l;
}

size_t
length(List l)
{
    size_t len;

    for (len = 0; !is_empty_list(l); l = cdr(l)) {
        ++len;
    }

    return len;
}

List
find_first(int jdg(void *), List l)
{
    while (!is_empty_list(l)) {
        if (CLOSURE(jdg)(car(l))) {
            return l;
        }
        l = cdr(l);
    }
    return empty_list;
}

void
map(void f(void *), List l)
{
    void *elm;
    FOREACH (elm, l) {
        CLOSURE(f)(elm);
    }
}

List
append(List *l1_r, List *l2_r)
{
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
    } while (0);

    l = *l1_r;

    *l1_r = empty_list;
    *l2_r = empty_list;

    return l;
}

void
free_list(List *l_r)
{
    List l = *l_r;

    while (!is_empty_list(l)) {
        List _l = l;
        l = cdr(l);
        FREE(_l);
    }

    *l_r = empty_list;
}

List
push(void *elm, List *l_r)
{
    return *l_r = cons(elm, *l_r);
}

void *
pop(List *l_r)
{
    List _l = cdr(*l_r);
    void *v = car(*l_r);
    (*l_r)->rest = empty_list;
    FREE(*l_r);
    *l_r = _l;
    return v;
}

void
remove_all(int jdg(void *), List *l_r)
{
    List tail, next;

    tail = empty_list;
    next = *l_r;
    while (!is_empty_list(next)) {
        if (CLOSURE(jdg)(car(next))) {
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

void *
remove_first(int jdg(void *), List *l_r)
{
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

void
reverse(List *l_r)
{
    List l, ll;

    l = *l_r;
    ll = empty_list;

    while (l != empty_list) {
	List l_t = l->rest;
	l->rest = ll;
	ll = l;
	l = l_t;
    }

    *l_r = ll;
}
