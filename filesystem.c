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
//todo (nicolas): add logica retorno
// errors:
// 0 - success
// 1 - permission denied
// 2 - file not found
int delete_file(bitmap * bitmap, const char * filename, process * owner) {
    file_system * fs = get_file_system();
    bool file_found = false;
    bool permission_denied = false;
    for (int i = 0; i < fs->file_count; i++) {
        file * f = fs->files[i];
        if (f != NULL && strcmp(f->name, filename) == 0) {
            file_found = true;
            if (f->pid != owner->pid && owner->priority != 0) {
                permission_denied = true;
                break;
            }
        }
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
            return 0;
        }
    }
    if (permission_denied) {
        return 1;
    }
    if (!file_found) { 
        return 2;
    }
    return 3; // Should not reach here
}

//todo (nicolas): add logica retorno
// errors:
// 0 - success
// 1 - not enough space
int add_file(bitmap * bitmap, const char * filename, int size, process * owner) {
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
                        return 0;
                    }
                }
            }
        } else {
            consecutive_free = 0;
        }
    }
    return 1;
    // If we reach here, allocation failed
}

void mount_file_system(bitmap * bitmap, file_list * files_to_mount) {
    file_system * fs = get_file_system();
    fs->total_space = bitmap->size;
    fs->files = (file **)calloc(fs->total_space, sizeof(file *));
    for (int i = 0; i < files_to_mount->files_count; i++) {
        file * f = files_to_mount->files[i];
        // Mark space in bitmap
        for (int j = f->start_offset; j < f->start_offset + f->size; j++) {
            bitmap->bits[j] = 1;
        }
        // Add file to file system
        for (int k = 0; k < fs->total_space; k++) {
            if (fs->files[k] == NULL) {
                fs->files[k] = f;
                fs->file_count++;
                break;
            }
        }
    }
}