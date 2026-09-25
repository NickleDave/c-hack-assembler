#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>

#include "parser.h"
#include "symbol_table.h"


void assemble(FILE *fp) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int rom_address = 0;
    SymbolTable *table = new_symbol_table();

    struct Command *parsed_commands = NULL;
    int num_ca_commands_parsed = 0;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        struct Command command = parse_line(line);
        switch (command.command_type)
        {
        case L_COMMAND:
            /* code */
            if (table_lookup_symbol(command.symbol, table) == -1)
                add_symbol_to_table(command.symbol, rom_address, table);
            break;
        case C_COMMAND:
        case A_COMMAND:
            // do the re-alloc dance
            num_ca_commands_parsed += 1;
            struct Command *new_parsed_commands_ptr = (struct Command*) realloc(
                    parsed_commands, num_ca_commands_parsed * sizeof(struct Command)
                );
            if (new_parsed_commands_ptr == NULL) {
                fprintf(stderr, "parser::first_pass: reallocation of `parsed_commands` failed");
                exit(EXIT_FAILURE);
            }
            parsed_commands = new_parsed_commands_ptr;
            parsed_commands[num_ca_commands_parsed - 1] = command;
            // and finally, increment `rom_address`
            rom_address += 1;
            break;
        }
    }

    free_symbol_table(table);
}
