#ifndef LIGHT_LIST_H
#define LIGHT_LIST_H

#include <stddef.h>

typedef struct list *List;

List cons(void *ele, List l);
void *car(List l);
List cdr(List l);
List list_from_arr(void *arr, size_t len, size_t size);

size_t length(List l);

void map(void f(void *), List l);

List append(List *l1_r, List *l2_r);
void free_list(List *l_r);

#define empty_list NULL
#define is_empty_list(l) ((l) == NULL)

#define LIST(arr, len) list_from_arr((arr), len, sizeof((arr)[0]))
#define FOREACH(e, l) for (List __l = (l); !is_empty_list(__l) && (((e) = (typeof(e)) car(__l)) != NULL); __l = cdr(__l))

#endif
