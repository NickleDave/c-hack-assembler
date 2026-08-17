#include <stdio.h>

#ifndef HACK_ASSEMBLER_PARSER_H
#define HACK_ASSEMBLER_PARSER_H

enum CommandType{
    A_COMMAND,
    C_COMMAND,
    L_COMMAND,
    COMMENT,
    EMPTY_LINE,  // after call to getline, buffer will contain "\n\0"
    ASM_EOF,
};

struct Command {
    enum CommandType command_type;
    char *symbol;
    char *dest;
    char *comp;
    char *jump;
};

// functional interface
struct Command parse_line(char line[]);
void first_pass(FILE *fp);

#endif