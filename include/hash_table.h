#ifndef LIGHT_HASH_TABLE
#define LIGHT_HASH_TABLE

#include <stddef.h>
#include <stdint.h>
#include "list.h"
#include "table_common.h"

typedef struct hash_table *HashTable;

HashTable new_hash_table(
    size_t n, uint32_t hf(const void *), int eq(const void *, const void *)
);
void free_hash_table(HashTable *tbl_r);
List get_all_records_from_hash_table(HashTable tbl);

void *get_from_hash_table(HashTable tbl, const void *key);
void put_to_hash_table(HashTable tbl, const void *key, void *v);
void *remove_from_hash_table(HashTable tbl, const void *key);

#endif
