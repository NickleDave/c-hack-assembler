#include <stdio.h>
#include <stdlib.h>

enum CommandType{
    A_COMMAND,
    C_COMMAND,
    L_COMMAND,
    COMMENT,
    EMPTY_LINE,
    ASM_EOF,
};


struct Parser {
    FILE *file;                // pointer to open file
    int line_number;           // current line in open file
    enum CommandType command_type;  // current command type
};


struct Parser new_parser(const char* filename) {
    struct Parser* p = NULL;
    p = (struct Parser*) malloc(sizeof(struct Parser));
    if (p == NULL) {
        return NULL;
    }
}

int has_more_commands(struct Parser* p) {
   if (feof(p->file)) return 0;
   else return 1;
}

void advance(struct Parser* p) {
    if (has_more_commands(p))
        p->line += 1;
    else
        
}