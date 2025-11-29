#pragma once

#include "types.h"
#include <stdlib.h>
#include <string.h>

file_system * get_file_system();
bitmap * new_bitmap(int size);

int delete_file(bitmap * bitmap, const char * filename, process * owner);

int add_file(bitmap * bitmap, const char * filename, int size, process * owner);
void mount_file_system(bitmap * bitmap, file_list * files_to_mount);
