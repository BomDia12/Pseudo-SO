#include "disk_reader.h"

int main (int argc, char *argv[]) {

    if (argc < 2) {
        printf("Error: Devem ser informados dois arquivos!\n");
        return 1;
    }

    read_procecss_file(argv[0]);

    return 0;
}