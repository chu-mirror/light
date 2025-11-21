#ifndef LIGHT_ARRAY_H
#define LIGHT_ARRAY_H

typedef struct array *Array;

void new_array(Array *arr_r);
void free_array(Array *arr_r);
void **nth(Array arr, int n);

#endif
