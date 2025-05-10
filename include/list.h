#ifndef LIGHT_LIST_H
#define LIGHT_LIST_H

#include <stddef.h>

typedef struct list *List;

#define empty_list NULL

List cons(void *elm, List l);
void *car(List l);
List cdr(List l);
List list_from_arr(void *arr, size_t len, size_t size);
List copy_list(List l);

size_t length(List l);
List find_first(int jdg(void *), List l);

void map(void f(void *), List l);

List append(List *l1_r, List *l2_r);
void free_list(List *l_r);
List push(void *elm, List *l_r);
void *pop(List *l_r);
void remove_all(int jdg(void *), List *l_r);
void *remove_first(int jdg(void *), List *l_r);

static inline void *
cadr(List l)
{
    return car(cdr(l));
}

static inline List
pair(void *elm1, void *elm2)
{
    return cons(elm1, cons(elm2, empty_list));
}

#define first_of car
#define second_of cadr

#define is_empty_list(l) ((l) == NULL)
#define _LIST(arr, len) list_from_arr((arr), len, sizeof((arr)[0]))
#define LIST(arr) _LIST((arr), sizeof(arr) / sizeof((arr)[0]))
#define FOREACH(elm, l)                                           \
    for (List __light_list_l = (l);                               \
         !is_empty_list(__light_list_l)                           \
         && (((elm) = (typeof(elm))car(__light_list_l)) != NULL); \
         __light_list_l = cdr(__light_list_l))

#endif
