#pragma once

#include "./types.h"
#include <string.h>

file_system * get_file_system();

void delete_file(const char * filename);

void add_file(const char * filename, int size);
