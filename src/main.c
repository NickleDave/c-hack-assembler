#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "assemble.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        puts("Invalid number of arguments");
    }

    FILE *fp;
    
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Assembler");
        return EXIT_FAILURE;
    }

    bool verbose;
    if (argv[2]) {
        verbose = true;
    } else {
        verbose = false;
    }

    assemble(fp, verbose);

}