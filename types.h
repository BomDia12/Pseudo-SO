#pragma once

#include <cstdint>

typedef struct {
    int priority;
    int curr_size;
    process * processes[100];
    int alloted_time;
} queue;

typedef struct {
    int pid;
    int memory_usage;
    int scanner_usage;
    int printer_usage;
    int modem_usage;
    int sata_usage;
    int entry_time;
    int curr_instruction;
    int priority;
    queue * curr_queue;
    usage_block * memory_block;
} process;

typedef struct {
    int offset;
    int size;
    process * user;
} usage_block;

typedef struct {
    int curr_time;
    bool preemptive_mode; // indicates whether preemptive scheduling is enabled
    int preemptive_time; // time to remove current process
    queue * rt_queue;
    queue * user_queue_1;
    queue * user_queue_2;
    queue * user_queue_3;
    queue * user_queue_4;
    queue * user_queue_5;
    queue * blocked_processes;
    int allocated_blocks; // number of allocated memory blocks (size of array bellow)
    usage_block ** memory_blocks; // array of pointers to memory blocks
    process * curr_process;
    resources system_resources;
} scheduler;

typedef struct {
    process ** processes;
    int process_count;
    int offset;
} process_list;

typedef struct {
    char * name;
    int start_offset;
    int size;
} file;

typedef struct {
    int total_space;
    file ** files; // nullptrr for empty, size = total_space
} file_system;

typedef struct {
    int value;
    char * name;
} semaphore;

typedef struct {
    semaphore scanner;
    semaphore printer_1;
    semaphore printer_2;
    semaphore modem;
    semaphore sata_1;
    semaphore sata_2;
    semaphore sata_3;
} resources;
