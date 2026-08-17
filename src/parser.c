#include <stdio.h>
#include <sys/types.h>

#include "parser.h"


struct Command parse_line(char line[]) {
    struct Command command;
    if (line == "\n\0") {
        struct Command command = {EMPTY_LINE};
        return command;
    }
}


void first_pass(FILE *fp) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        struct Command command = parse_line(line);
    }

}