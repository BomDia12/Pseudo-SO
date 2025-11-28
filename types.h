#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct process process;
typedef struct queue queue;
typedef struct usage_block usage_block;
typedef struct process_list process_list;
typedef struct fs_file fs_file;
typedef struct file_system file_system;
typedef struct semaphore semaphore;
typedef struct resources resources;
typedef struct scheduler scheduler;
typedef struct file file;

struct queue {
    int priority;
    int curr_size;
    process * processes[100];
    int alloted_time;
} ;

struct process{
    int pid;
    int entry_time;
    int priority;
    usage_block * memory_block;
    int scanner_usage;
    int printer_usage;
    int modem_usage;
    int memory_usage;
    int sata_usage;
    int curr_instruction;
    queue * curr_queue;
} ;

struct usage_block{
    int offset;
    int size;
    process * user;
} ;

struct scheduler{
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
    resources* system_resources;
} ;

struct process_list {
    process ** processes;
    int process_count;
    int offset;
} ;

struct file{
    char * name;
    int start_offset;
    int size;
    int pid; // owner process id
    bool user_file;
};

struct file_system{
    int total_space;
    int file_count;
    file ** files; // nullptrr for empty, size = total_space
};

struct semaphore{
    int value;
    char * name;
} ;

struct resources{
    semaphore scanner;
    semaphore printer_1;
    semaphore printer_2;
    semaphore modem;
    semaphore sata_1;
    semaphore sata_2;
    semaphore sata_3;
};

typedef struct {
    int * bits;
    int size;
} bitmap;
