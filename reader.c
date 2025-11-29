#include "reader.h"

int get_number_of_lines(FILE* f) {
    int number_of_lines = 0;
    char c;
    while ((c = fgetc(f)) != EOF) {
        if (c == '\n') {
            number_of_lines++;
        }
    }
    return number_of_lines;
}

process_list read_process_file(const char * file_name) {
    FILE* processes_file = fopen(file_name, "r");
    int number_of_process = get_number_of_lines(processes_file);
    rewind(processes_file);
    process** processes = malloc(sizeof(process*) * number_of_process);
    char line[256];
    for(int i =0; i < number_of_process; i++) {
        fgets(line, sizeof(line), processes_file);
        int entry_time, priority, total_instructions, scanner, printer, modem, mem_usage, sata;
        sscanf(
            line,
            "%d,%d,%d,%d,%d,%d,%d,%d",
            &entry_time,
            &priority,
            &total_instructions,
            &mem_usage,
            &scanner,
            &printer,
            &modem,
            &sata
        );
        processes[i] = malloc(sizeof(process));
        processes[i]->pid = i;
        processes[i]->entry_time = entry_time;
        processes[i]->priority = priority;
        processes[i]->total_instructions = total_instructions;
        processes[i]->memory_usage = mem_usage;
        processes[i]->memory_block = NULL;
        processes[i]->scanner_usage = scanner;
        processes[i]->printer_usage = printer;
        processes[i]->modem_usage = modem;
        processes[i]->sata_usage = sata;
        processes[i]->curr_instruction = 0;
        processes[i]->last_promotion_time = entry_time;
        processes[i]->curr_queue = NULL;
    }
        
    fclose(processes_file);
    process_list list = {
        .processes = processes,
        .process_count = number_of_process,
        .offset = 0   
    };
    return list;
}

file_system_manager read_instructions_file(const char *file_name) {
    FILE* instructions_file = fopen(file_name, "r");
    int number_of_lines = get_number_of_lines(instructions_file);
    rewind(instructions_file);
    char line[256];
    int blocks_on_disk, segments_num;

    fgets(line, sizeof(line), instructions_file);
    sscanf(line, "%d", &blocks_on_disk);


    fgets(line, sizeof(line), instructions_file);
    sscanf(line, "%d", &segments_num);

    file ** files = malloc(sizeof(file *) * segments_num);
    for(int i =0; i < segments_num; i++) {
        fgets(line, sizeof(line), instructions_file);
        char file_name;
        int first_block, number_of_blocks;
        sscanf(
            line,
            "%c,%d,%d",
            &file_name,
            &first_block,
            &number_of_blocks
        );
        files[i] = malloc(sizeof(file));
        files[i]->name = &file_name;
        files[i]->pid = -1;
        files[i]->start_offset = first_block;
        files[i]->size = number_of_blocks;
    }

    int number_of_operations = number_of_lines - segments_num - 2;
    operation ** operations_list = malloc(sizeof(operation *) * number_of_operations);
    for(int i =0; i < number_of_operations; i++) {
        fgets(line, sizeof(line), instructions_file);
        char file_name;
        int pid, opcode, number_blocks_to_create;
        sscanf(
            line,
            "%d,%d,%c,%d",
            &pid,
            &opcode,
            &file_name,
            &number_blocks_to_create
        );
        operations_list[i] = malloc(sizeof(operation));
        operations_list[i]->pid = pid;
        operations_list[i]->opcode=opcode;
        operations_list[i]->file_name=&file_name;
        operations_list[i]->blocks_to_create = number_blocks_to_create;
    }

    fclose(instructions_file);
    file_system_manager manager = {
        .num_total_blocks = blocks_on_disk,
        .file_list = {
            .files_count = segments_num,
            .files = files
        },
        .operations_list = operations_list

    };
    return manager;
}