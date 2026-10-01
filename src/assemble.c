#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>

#include "parser.h"
#include "symbol_table.h"
#include "assemble.h"


void assemble(FILE *fp, bool verbose) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int rom_address = 0;
    SymbolTable *table = new_symbol_table();

    Command **parsed_commands = NULL;
    int num_ca_commands_parsed = 0;

    // first pass: parse all lines, and add all L-commands to symbol table
    while ((read = getline(&line, &len, fp)) != -1)
    {
        Command *command = parse_line(line);
        switch (command->command_type)
        {
        case L_COMMAND:
            /* code */
            if (table_lookup_symbol(command->symbol, table) == -1)
                add_symbol_to_table(command->symbol, rom_address, table);
            free_command(command);
            break;
        case C_COMMAND:
        case A_COMMAND:
            // do the re-alloc dance
            num_ca_commands_parsed += 1;
            Command **new_parsed_commands_ptr = (Command**) realloc(
                    parsed_commands, num_ca_commands_parsed * sizeof(Command)
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
        case COMMENT:
        case EMPTY_LINE:
        case ASM_EOF:
        default:
            free_command(command);
            break;
        }
    }

    if (verbose) {
        printf("Parsed commands:\n");
        for (int i=0; i < num_ca_commands_parsed; i++) {
            print_command(parsed_commands[i]);
        }
        printf("\n");
        printf("Symbol table:\n");
        for (int i=0; i < table->len; i++) {
            printf("\tsymbol=%s, address=%d\n", table->pairs[i]->symbol, table->pairs[i]->rom_address);
        }
    }

    // second pass
    int ram_address = 16;
    char **binary_lines;
    for (int i=0; i < num_ca_commands_parsed; i++) {
        Command *command = parsed_commands[i];
        switch (command->command_type)
        {
        case A_COMMAND:
            // FIXME: just realized we need constant to default
            // to something that is not zero

            // and finally, increment `rom_address`
            rom_address += 1;
            break;   

        case C_COMMAND:
            // FIX ME: 
            line = "";
    }

    // clean up
    for (int i=0; i<num_ca_commands_parsed; i++) {
        free_command(parsed_commands[i]);
    }
    free_symbol_table(table);
}