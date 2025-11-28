#pragma once

#include "./types.h"
#include <string.h>

file_system * get_file_system();
bitmap * new_bitmap(int size);

void delete_file(bitmap * bitmap, const char * filename, process * owner);

void add_file(bitmap * bitmap, const char * filename, int size, process * owner);
