#include "disk_reader.h"

int get_number_of_lines(FILE* f) {
    int number_of_lines = 0;
    while ((c = fgetc(f)) != EOF) {
        if (c == '\n') {
            number_of_lines++;
        }
    }
    return number_of_lines;
}



process** read_process_file(string file_name) {
    file* processes_file = fopen(file_name, "r");
    int number_of_process = get_number_of_lines(processes_file);
    **process processes = malloc(sizeof(*process) * number_of_process);
    string line;
    for(int i =0; i < number_of_process; i++) {
        fgets(line, sizeof(line), processes_file);
        processes[i] = maloc(process);
        processes[i]->id = i;
        processes[i]->entry_time = ;
        processes[i]->priority =;
        processes[i]->memory_block = maloc(usage_block)
        processes[i]->memory_block->size = ;
        //todo (gabriel): preencher informações memory_block
        processes[i]->scanner_usage =;
        processes[i]->printer_usage =;
        processes[i]->modem_usage =;
        processes[i]->memory_usage =;
        processes[i]->sata_usage =;
        //todo (gabriel): preencher informações curr_instr e curr_queue
        processes[i]->curr_instruction = maloc(int);
        pricesses[i]->curr_queue = maloc(queue);
    }
        
    fclose(processes_file);
    return processes;
}