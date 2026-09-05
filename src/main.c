#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        puts("Invalid number of arguments");
    }

    FILE *fp;
    
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Assembler");
        return EXIT_FAILURE;
    }

}