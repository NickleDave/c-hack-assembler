#include <stdbool.h>
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
    int constant;
    char *dest;
    char *comp;
    char *jump;
};

// functional interface
void remove_spaces(char str_trimmed[], char str_untrimmed[]);
bool is_all_digits(char str[]);
int constant_str_to_int(char * constant_str);
bool is_valid_symbol(char str[]);
int char_count(char * str, char char_);
struct Command parse_line(char line[]);
void first_pass(FILE *fp);

#endif