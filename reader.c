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



process** read_process_file(const char * file_name) {
    FILE* processes_file = fopen(file_name, "r");
    int number_of_process = get_number_of_lines(processes_file);
    rewind(processes_file);
    process** processes = malloc(sizeof(process*) * number_of_process);
    char line[256];
    for(int i =0; i < number_of_process; i++) {

        int entry_time, priority ,mem_size, scanner, printer, modem, mem_usage, sata;
        fgets(line, sizeof(line), processes_file);
        sscanf(
            line,
            "%d,%d,%d,%d,%d,%d,%d,%d",
            &entry_time,
            &priority,
            &mem_size,
            &scanner,
            &printer,
            &modem,
            &mem_usage,
            &sata
        );
        printf("linha %s", line);
        processes[i] = malloc(sizeof(process));
        processes[i]->pid = i;
        processes[i]->entry_time = entry_time;
        processes[i]->priority =priority;
        processes[i]->memory_block = malloc(sizeof(usage_block));
        processes[i]->memory_block->size = mem_size;
        //todo (gabriel): preencher informações memory_block
        processes[i]->scanner_usage =scanner;
        processes[i]->printer_usage =printer;
        processes[i]->modem_usage =modem;
        processes[i]->memory_usage =mem_usage;
        processes[i]->sata_usage =sata;
        //todo (gabriel): preencher informações curr_instr e curr_queue
        processes[i]->curr_instruction = i;
        processes[i]->curr_queue = malloc(sizeof(queue));
    }
        
    fclose(processes_file);
    return processes;
}