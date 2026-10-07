#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>

#include "code.h"
#include "parser.h"
#include "symbol_table.h"

#include "assemble.h"


#define BIN_STR_LEN 17

// `int_to_bin_str` adapted from
// https://github.com/sahillathwal/c-hack-assembler/blob/main/main.c#L19

char *int_to_bin_str(int int_value) {
    char* bin_str = malloc(sizeof(char) * BIN_STR_LEN);
    bin_str[0] = '0';  // most significant bit is always 0, to indicate A-instruction
    bin_str[16] = '\0'; // null terminator, because this is a "string"

    for (int i = 0; i < BIN_STR_LEN - 2; i++) {
        // next line: bitwise and with 1 isolates the lowest bit,
        // i.e., the least significant digit.
        // if the number is even, this will be 0. If odd, 1.
        // We then add the result to '0' to get the char, either '0'
        // if the result was 0, or '1' if the results was 1
        bin_str[BIN_STR_LEN - 2 - i] = (int_value & 1) + '0';
        // bit-shift right; since we know `int_value` must always be positive
        // (required for string constants + enforced by ram/rom address scheme)
        // this will be an arithmetic shift that fills the new bit on the left
        // with zero.    char bin_str[17];
        int_value >>= 1;
    }

    return bin_str;
}


BinaryStrings assemble(FILE *fp, bool verbose) {
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
            printf("\tsymbol=%s, address=%d\n", table->pairs[i]->symbol, table->pairs[i]->address);
        }
    }

    // second pass
    int ram_address = 16;
    SymbolTable *predef_table = get_predefined_table();
    char** binary_lines = malloc(sizeof(char*) * num_ca_commands_parsed);
    for (int line_num=0; line_num < num_ca_commands_parsed; line_num++) {
        Command *command = parsed_commands[line_num];
        if (command->command_type == A_COMMAND)
        {
            if ((command->constant > -1) && !(strcmp(command->symbol, ""))) {
                char* bin_line = int_to_bin_str(command->constant);
                binary_lines[line_num] = bin_line;
            } else {
                int address;

                address = table_lookup_symbol(command->symbol, predef_table);
                if (address > -1) {
                    char* bin_line = int_to_bin_str(address);
                    binary_lines[line_num] = bin_line;
                    continue;
                }

                address = table_lookup_symbol(command->symbol, table);
                if (address > -1) {
                    char* bin_line = int_to_bin_str(address);
                    binary_lines[line_num] = bin_line;
                    continue;
                }

                // if we make it here, we didn't find the symbol in either table
                // so we need to add it
                add_symbol_to_table(command->symbol, ram_address, table);
                ram_address += 1;
            }

        } else if (command->command_type == C_COMMAND) {
            char *bin_line = malloc(sizeof(char) * BIN_STR_LEN);
            snprintf(
                bin_line,
                (sizeof(char) * BIN_STR_LEN),
                "%s%s%s%s",
                "111",
                comp_bits_str_from_mnemonic(command->comp),
                dest_bits_str_from_mnemonic(command->dest),
                jump_bits_str_from_mnemonic(command->jump)
            );
            binary_lines[line_num] = bin_line;
        } else {
            // this should never happen
            fprintf(stderr, "assemble: unexpected command type in second pass: %d", command->command_type);
            exit(EXIT_FAILURE);
        }
    }

    // clean up
    for (int i=0; i<num_ca_commands_parsed; i++) {
        free_command(parsed_commands[i]);
    }
    free_symbol_table(table);

    BinaryStrings out = {
        .num_strings=num_ca_commands_parsed,
        .strings=binary_lines,
    };
    return out;
}