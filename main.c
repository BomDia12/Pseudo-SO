#include "reader.h"
#include "printf.h"
int main (int argc, char *argv[]) {

    if (argc < 2) {
        printf("Error: Devem ser informados dois arquivos!\n");
        return 1;
    }
    printf("Bem vindo, querido(a)!\n");
    process_list list = read_process_file(argv[1]);
    print_processes(list);
    return 0;
}