#pragma once

#include "types.h"
#include <stdlib.h>
#include <string.h>

file_system * get_file_system();
bitmap * new_bitmap(int size);

int delete_file(bitmap * bitmap, const char * filename, process * owner);

int add_file(bitmap * bitmap, const char * filename, int size, process * owner);
