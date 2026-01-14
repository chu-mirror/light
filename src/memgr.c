#include "memgr.h"

size_t _light_alloc_count;
size_t _light_reserved_count;
bool _light_reserving;
size_t _light_doubled_reserved_count;
size_t _doubled_count;
size_t *const _light_global_doubled_count_r = &_doubled_count;
