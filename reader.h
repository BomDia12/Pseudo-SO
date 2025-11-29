#ifndef READER_H
#define READER_H

#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_number_of_lines(FILE *f);

process_list read_process_file(const char *file_name);

file_system_manager read_instructions_file(const char *file_name);


#endif
