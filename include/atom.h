#ifndef LIGHT_ATOM_H
#define LIGHT_ATOM_H

#include <stddef.h>
#include <stdint.h>

void *atom(const uint8_t *v, size_t len);
void *atom_str(const char *str);
void free_atom(const uint8_t *blk, size_t len);
void free_atom_str(const char *str);
void clear_atoms();

#endif
