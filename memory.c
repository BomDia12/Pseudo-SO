#include "./memory.h"

bitmap * get_user_bitmap() {
    static int bits_array[960] = {0};
    static bitmap mem_bitmap = { bits_array, 960 };
    return &mem_bitmap;
}

bitmap * get_realtime_bitmap() {
    static int bits_array[64] = {0};
    static bitmap mem_bitmap = { bits_array, 64 };
    return &mem_bitmap;
}
//TODO (nicolas): add logica 
// errors:
// 0 - success
// 1 - not enough space
// 2 - requesting more memory than available
int allocate_memory(process * proc) {
    if (proc->priority == 0 && proc->memory_usage > 64) {
        return 2;
    }
    if (proc->memory_usage > 960) {
        return 2;
    }
    bitmap * mem_bitmap;
    int required_blocks = proc->memory_usage;
    if (proc->priority == 0) {
        mem_bitmap = get_realtime_bitmap();
    } else {
        mem_bitmap = get_user_bitmap();
    }
    int consecutive_free = 0;
    int start_index = -1;
    for (int i = 0; i < mem_bitmap->size; i++) {
        if (mem_bitmap->bits[i] == 0) {
            if (consecutive_free == 0) {
                start_index = i;
            }
            consecutive_free++;
            if (consecutive_free == required_blocks) {
                // Allocate memory
                for (int j = start_index; j < start_index + required_blocks; j++) {
                    mem_bitmap->bits[j] = 1;
                }
                usage_block * block = (usage_block *)malloc(sizeof(usage_block));
                block->offset = start_index;
                block->size = required_blocks;
                block->user = proc;
                proc->memory_block = block;
                return 0;
            }
        } else {
            consecutive_free = 0;
        }
    }
    // If we reach here, allocation failed
    proc->memory_block = NULL;
    return 1;
}

void remove_memory(process * proc) {
    bitmap * mem_bitmap;
    if (proc->priority == 0) {
        mem_bitmap = get_realtime_bitmap();
    } else {
        mem_bitmap = get_user_bitmap();
    }
    for (int i = proc->memory_block->offset; i < proc->memory_block->offset + proc->memory_block->size; i++) {
        mem_bitmap->bits[i] = 0;
    }
    free(proc->memory_block);
    proc->memory_block = NULL;
}