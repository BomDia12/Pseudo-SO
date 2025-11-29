#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct process process;
typedef struct queue queue;
typedef struct queue_item queue_item;
typedef struct blocked_processes blocked_processes;
typedef struct semaphore semaphore;
typedef struct resources resources;
typedef struct scheduler scheduler;
typedef struct usage_block usage_block;
typedef struct process_list process_list;
typedef struct file_system file_system;
typedef struct file file;
typedef struct bitmap bitmap;
typedef struct file_list file_list;
typedef struct operation operation;
typedef struct file_system_manager file_system_manager;

struct queue {
    int priority;
    int curr_size;
    queue_item * first;
    queue_item * last;
    int alloted_time;
    bool preemptive;
};

struct queue_item {
    process * process;
    queue_item * next;
};

struct blocked_processes {
    process ** processes;
    int count;
};

struct process{
    int pid;
    int entry_time;
    int priority;
    int total_instructions;
    int memory_usage;
    usage_block * memory_block;
    int scanner_usage;
    int printer_usage;
    int modem_usage;
    int sata_usage;
    int curr_instruction;
    int last_promotion_time;
    queue * curr_queue;
};

struct usage_block{
    int offset;
    int size;
    process * user;
};

struct semaphore {
    int value;
    char * name;
};

struct resources {
    semaphore scanner;
    semaphore printer_1;
    semaphore printer_2;
    semaphore modem;
    semaphore sata_1;
    semaphore sata_2;
    semaphore sata_3;
};

struct scheduler{
    int curr_time;
    bool preemptive_mode; // indicates whether preemptive scheduling is enabled
    int preemptive_time; // time to remove current process
    queue rt_queue;
    queue user_queue_1;
    queue user_queue_2;
    queue user_queue_3;
    queue user_queue_4;
    queue user_queue_5;
    blocked_processes blocked_processes;
    int allocated_blocks; // number of allocated memory blocks (size of array bellow)
    usage_block ** memory_blocks; // array of pointers to memory blocks
    process * curr_process;
    resources system_resources;
};

struct process_list {
    process ** processes;
    int process_count;
    int offset;
} ;

struct file {
    char * name;
    int start_offset;
    int size;
    int pid; // owner process id
    bool user_file;
};

struct file_system {
    int total_space;
    int file_count;
    file ** files; // nullptrr for empty, size = total_space
};

struct file_list {
    file ** files;
    int files_count;
};

struct bitmap {
    int * bits;
    int size;
};

struct operation {
    int pid;
    int opcode;
    char * file_name;
    int blocks_to_create;
};

struct file_system_manager {
    int num_total_blocks;
    file_list file_list;
    operation ** operations_list;
};