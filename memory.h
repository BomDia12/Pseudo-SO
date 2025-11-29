#pragma once

#include "types.h"
#include <string.h>
#include <stdlib.h>

bitmap * get_user_bitmap();
bitmap * get_realtime_bitmap();

int allocate_memory(process * proc);
void remove_memory(process * proc);