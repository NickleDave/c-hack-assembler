#define _GNU_SOURCE

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol_table.h"


SymbolAddressPair *new_pair(char * symbol, int address) {
    SymbolAddressPair *pair = malloc(sizeof(SymbolAddressPair));
    if (pair == NULL) {
        fprintf(stderr, "symbol_table::new_pair: malloc failed for new SymbolAddressPair");
        exit(EXIT_FAILURE);
    }
    asprintf(&pair->symbol, "%s", symbol);  // we use `asprintf` as a safer version of `strncpy`
    pair->address = address;
    return pair;
}


void free_pair(SymbolAddressPair *pair) {
    free(pair->symbol); // because we make `symbol` with `asprintf`
    free(pair);
}


SymbolTable *new_symbol_table(void) {
    SymbolTable *out = malloc(sizeof(SymbolTable));
    if (out == NULL) {
        fprintf(stderr, "symbol_table::new_symbol_table: malloc of table failed");
        exit(EXIT_FAILURE);
    }
    *out = (SymbolTable) { };
    return out;
}


void free_symbol_table(SymbolTable *table) {
    for (int i = 0; i < table->len; i++) {
        free_pair(table->pairs[i]);
    }
    free(table->pairs);
    free(table);
}


int table_lookup_symbol(char *symbol, SymbolTable *table) {
    for (int i = 0; i < table->len; i++) {
        if (!strcmp(table->pairs[i]->symbol, symbol)) {
            return table->pairs[i]->address;
        }
    }
    // -1 is sentinel value that means "symbol not found".
    // this works well enough, since -1 is not a valid ROM address
    return -1;
}


int add_symbol_to_table(char *symbol, int address, SymbolTable* table) {
    // pre-condition: symbol can't already be in table, return fail if it is
    if (!(table_lookup_symbol(symbol, table) == -1)) {
        return -1;
    }

    // ok, now add to table
    table->len++;
    struct SymbolAddressPair** new_pairs_ptr = realloc(
        table->pairs, sizeof(SymbolAddressPair*) * table->len
    );
    if (new_pairs_ptr == NULL) {
        fprintf(stderr, "symbol_table::add_symbol_to_table: realloc of table->pairs failed");
        exit(EXIT_FAILURE);
    }
    table->pairs = new_pairs_ptr;

    SymbolAddressPair *new_pair_ptr = new_pair(symbol, address);
    table->pairs[table->len - 1] = new_pair_ptr;

    return 0;
}


SymbolTable *get_predefined_table(void) {
    SymbolTable *table = new_symbol_table();
    add_symbol_to_table("SP", 0, table);
    add_symbol_to_table("LCL", 1, table);
    add_symbol_to_table("ARG", 2, table);
    add_symbol_to_table("THIS", 3, table);
    add_symbol_to_table("THAT", 4, table);
    add_symbol_to_table("R0", 0, table);
    add_symbol_to_table("R1", 1, table);
    add_symbol_to_table("R2", 2, table);
    add_symbol_to_table("R3", 3, table);
    add_symbol_to_table("R4", 4, table);
    add_symbol_to_table("R5", 5, table);
    add_symbol_to_table("R6", 6, table);
    add_symbol_to_table("R7", 7, table);
    add_symbol_to_table("R8", 8, table);
    add_symbol_to_table("R9", 9, table);
    add_symbol_to_table("R10", 10, table);
    add_symbol_to_table("R11", 11, table);
    add_symbol_to_table("R12", 12, table);
    add_symbol_to_table("R13", 13, table);
    add_symbol_to_table("R14", 14, table);
    add_symbol_to_table("R15", 15, table);
    add_symbol_to_table("SCREEN", 16384, table);
    add_symbol_to_table("KBD", 24576, table);
    return table;
}

