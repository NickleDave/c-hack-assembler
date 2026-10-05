#include <stdbool.h>
#include <stdio.h>

#ifndef ASSEMBLE_H
#define ASSEMBLE_H

typedef struct {
    int num_strings;
    char** strings;
} BinaryStrings;

BinaryStrings assemble(FILE *fp, bool verbose);

#endif
