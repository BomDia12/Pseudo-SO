#ifndef READER_H
#define READER_H

#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

process **read_process_file(const char *file_name);
int get_number_of_lines(FILE *f);

#endif
