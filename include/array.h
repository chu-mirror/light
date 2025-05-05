#ifndef LIGHT_ARRAY_H
#define LIGHT_ARRAY_H

typedef struct array *Array;

Array new_array();
void free_array(Array arr);
void **nth(Array arr, int n);

#endif
