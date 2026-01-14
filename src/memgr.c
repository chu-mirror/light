#include "memgr.h"

size_t _light_alloc_count;
size_t _light_reserved_count;
struct light_reserving_frame *_light_current_reserving_frame;
