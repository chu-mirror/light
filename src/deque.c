#include <string.h>

#include "light.h"
#include "array.h"

#include "deque.h"

struct deque {
    Array array;
    int head;
    int tail;
    int size;
};

static inline int
next_slot_back(Deque q)
{
    return (q->tail + 1) % q->size;
}

static inline int
previous_slot_back(Deque q)
{
    return (q->tail - 1 + q->size) % q->size;
}

static inline int
next_slot_front(Deque q)
{
    return (q->head - 1 + q->size) % q->size;
}

static inline int
previous_slot_front(Deque q)
{
    return (q->head + 1) % q->size;
}

static inline int
used_slots(Deque q)
{
    return (q->tail - q->head + q->size) % q->size;
}

static inline int
is_full_deque(Deque q)
{
    return used_slots(q) == q->size - 1;
}

static void
enlarge_deque(Deque q)
{
    int ns = q->size << 1; /* new size */
    int sht = q->size; /* shift */
    nth(q->array, ns - 1); /* ensure that array[0..ns) is accessable */
    if (q->head > q->tail) {
        memcpy(
            nth(q->array, q->head) + sht,
            nth(q->array, q->head),
            (q->size - q->head) * sizeof(void *)
        );
        q->head += sht;
    }
    q->size = ns;
}

Deque
new_deque()
{
    Deque q;
    NEW(q);
    q->array = new_array();
    q->head = 0;
    q->tail = 0;
    q->size = 2;
    return q;
}

void
free_deque(Deque *q_r)
{
    free_array(&(*q_r)->array);
    FREE(*q_r);
    *q_r = NULL;
}

int
is_empty_deque(Deque q)
{
    return q->tail == q->head;
}

void
push_back(Deque q, void *elm)
{
    if (is_full_deque(q)) {
        enlarge_deque(q);
    }
    *nth(q->array, q->tail) = elm;
    q->tail = next_slot_back(q);
}

void
push_front(Deque q, void *elm)
{
    if (is_full_deque(q)) {
        enlarge_deque(q);
    }
    q->head = next_slot_front(q);
    *nth(q->array, q->head) = elm;
}

void *
pop_back(Deque q)
{
    if (is_empty_deque(q)) {
        return NULL;
    }
    q->tail = previous_slot_back(q);
    return *nth(q->array, q->tail);
}

void *
pop_front(Deque q)
{
    if (is_empty_deque(q)) {
        return NULL;
    }

    void *elm = *nth(q->array, q->head);
    q->head = previous_slot_front(q);
    return elm;
}

void **
nth_of_deque(Deque q, int n)
{
    if (n > used_slots(q)) {
        return NULL;
    }
    return nth(q->array, (q->head + n) % q->size);
}
