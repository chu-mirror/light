#ifndef LIGHT_DEQUE_H
#define LIGHT_DEQUE_H

typedef struct deque *Deque;

Deque new_deque();
void free_deque(Deque *q_r);
void clear_deque(Deque q);
int is_empty_deque(Deque q);
int length_of_deque(Deque q);
void push_back(Deque q, void *elm);
void push_front(Deque q, void *elm);
void *pop_back(Deque q);
void *pop_front(Deque q);
void **nth_of_deque(Deque q, int n);

#endif
