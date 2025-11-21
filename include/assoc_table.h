#ifndef LIGHT_ASSOC_TABLE
#define LIGHT_ASSOC_TABLE

typedef struct assoc_table *AssocTable;

void new_assoc_table(AssocTable *tbl_r, int eq(const void *, const void *));
void free_assoc_table(AssocTable *tbl_r);

void *get_from_assoc_table(AssocTable tbl, const void *key);
void put_to_assoc_table(AssocTable tbl, const void *key, void *v);
void remove_from_assoc_table(AssocTable tbl, const void *key);

#endif
