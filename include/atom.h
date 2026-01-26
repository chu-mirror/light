#ifndef LIGHT_ATOM_H
#define LIGHT_ATOM_H

#include <stddef.h>
#include <stdint.h>

void *atom(const uint8_t *v, size_t len);
const char *atom_str(const char *str);

#endif
