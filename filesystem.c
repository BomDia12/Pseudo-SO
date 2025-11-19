#include "./filesystem.h"

file_system * get_file_system() {
    static file_system fs = {0, 0, NULL};
    return &fs;
}
