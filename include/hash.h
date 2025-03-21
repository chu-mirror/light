#ifndef LIGHT_HASH_H
#define LIGHT_HASH_H

#include <stddef.h>
#include <stdint.h>

uint32_t hash(const uint8_t *value, size_t len);
uint32_t hash_str(const char *str);

#endif
