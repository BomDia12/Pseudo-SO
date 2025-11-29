#include "reader.h"
#include "scheduler.h"

int main (int argc, char *argv[]) {

    if (argc < 2) {
        printf("Error: Devem ser informados dois arquivos!\n");
        return 1;
    }
    printf("Bem vindo, querido(a)!\n");
    process_list processes =  read_process_file(argv[1]);
    file_system_manager manager = read_instructions_file(argv[2]);
    scheduler_process(&processes);

    return 0;
}