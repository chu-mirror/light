#include <string.h>

#include "hash.h"

uint32_t
hash(const uint8_t *value, size_t len)
{
    const uint32_t p = 257;
    uint32_t result, p_;
    int i;

    for (i = 0, p_ = p, result = 0; i < len; ++i) {
        result += *(value + i) * p_;
        p_ *= p;
    }
    return result;
}

uint32_t
hash_str(const char *str)
{
    return hash((const uint8_t *)str, strlen(str));
}
