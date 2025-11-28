#include "./filesystem.h"

file_system * get_file_system() {
    static file_system fs = {0, 0, NULL};
    return &fs;
}

bitmap * new_bitmap(int size) {
    bitmap * bm = (bitmap *)malloc(sizeof(bitmap));
    bm->size = size;
    bm->bits = (int *)calloc(size, sizeof(int));
    return bm;
}

void delete_file(bitmap * bitmap, const char * filename, process * owner) {
    file_system * fs = get_file_system();
    for (int i = 0; i < fs->total_space; i++) {
        file * f = fs->files[i];
        if (f != NULL && strcmp(f->name, filename) == 0 && (f->pid == owner->pid || owner->priority == 0)) {
            // Free the space in bitmap
            for (int j = f->start_offset; j < f->start_offset + f->size; j++) {
                bitmap->bits[j] = 0;
            }
            // Remove file from file system
            free(f->name);
            free(f);
            fs->files[i] = NULL;
            fs->file_count--;
            return;
        }
    }
}

void add_file(bitmap * bitmap, const char * filename, int size, process * owner) {
    file_system * fs = get_file_system();
    int consecutive_free = 0;
    int start_index = -1;
    for (int i = 0; i < bitmap->size; i++) {
        if (bitmap->bits[i] == 0) {
            if (consecutive_free == 0) {
                start_index = i;
            }
            consecutive_free++;
            if (consecutive_free == size) {
                // Allocate space in bitmap
                for (int j = start_index; j < start_index + size; j++) {
                    bitmap->bits[j] = 1;
                }
                // Create new file
                file * new_file = (file *)malloc(sizeof(file));
                new_file->name = (char *)malloc(strlen(filename) + 1);
                strcpy(new_file->name, filename);
                new_file->start_offset = start_index;
                new_file->size = size;
                new_file->pid = owner->pid;
                new_file->user_file = (owner->priority != 0);
                // Add file to file system
                for (int k = 0; k < fs->total_space; k++) {
                    if (fs->files[k] == NULL) {
                        fs->files[k] = new_file;
                        fs->file_count++;
                        return;
                    }
                }
                // If we reach here, no space in file system array
                free(new_file->name);
                free(new_file);
                return;
            }
        } else {
            consecutive_free = 0;
        }
    }
    // If we reach here, allocation failed
}