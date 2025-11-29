#include "print_info.h"

void print_processes(process_list process_list) {
    for(int i = 0; i < process_list.process_count; i++) {
        print_process_info(process_list.processes[i]);
    }
}

//TODO: implementar offset na impressao
void print_process_info(process* process) {
    printf("dispatcher =>\n");
    printf("Pid: %d\n", process->pid);
    printf("offset: %d\n", 0);
    printf("blocks: %d\n", process->memory_usage);
    printf("priority: %d\n", process->priority);
    printf("time: %d\n", process->entry_time);
    printf("scanners: %d\n", process->scanner_usage);
    printf("printers: %d\n", process->printer_usage);
    printf("modems: %d\n", process->modem_usage);
    printf("sata: %d\n", process->sata_usage);
    printf("-------------------------------\n\n");
}


