#include "light.h"
#include "string.h"

#include "array.h"

#define INITIAL_ARRAY_SIZE 64

struct array {
    void **elements;
    int size;
};

Array
new_array()
{
    Array arr;
    NEW(arr);
    arr->size = INITIAL_ARRAY_SIZE;
    CALLOC(arr->elements, INITIAL_ARRAY_SIZE);
    return arr;
}

void
free_array(Array *arr_r)
{
    FREE((*arr_r)->elements);
    FREE(*arr_r);
}

void **
nth(Array arr, int n)
{
    if (n < arr->size) {
        return &arr->elements[n];
    }

    int _size = arr->size;
    while (!(n < arr->size)) {
        arr->size <<= 1;
    }
    REALLOC_ARRAY(arr->elements, arr->size);

    ZEROFY(arr->elements + _size, arr->size - _size);

    return &arr->elements[n];
}
