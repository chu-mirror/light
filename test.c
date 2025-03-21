#include <stdio.h>

#include "hash.h"

int main() {
    printf("Hello's hash is %08X\n", hash((const uint8_t *)"Hello", 5));
    printf("Hello's hash is %08X\n", hash_str("Hello"));
    return 0;
}
