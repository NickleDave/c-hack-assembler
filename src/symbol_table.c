#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol_table.h"


SymbolAddressPair* new_pair(char * symbol, int address) {
    SymbolAddressPair *pair = malloc(sizeof(SymbolAddressPair) + sizeof(symbol));
    if (pair == NULL) {
        fprintf(stderr, "symbol_table::new_pair: malloc failed for new SymbolAddressPair");
        exit(EXIT_FAILURE);
    }
    memcpy(pair->symbol, symbol, sizeof(*symbol));
    pair->rom_address = address;
    return pair;
}


void free_pair(SymbolAddressPair *pair) {
    free(pair);
}


SymbolTable* new_symbol_table(void) {
    SymbolTable * out = malloc(sizeof(SymbolTable));
    if (out == NULL) {
        fprintf(stderr, "symbol_table::new_symbol_table: malloc of table failed");
        exit(EXIT_FAILURE);
    }
    *out = (SymbolTable) { };
    return out;
}


void free_symbol_table(SymbolTable * table) {
    for (int i = 0; i < table->len; i++) {
        free_pair(table->pairs[i]);
    }
    free(table->pairs);
    free(table);
}


int table_lookup_symbol(char * symbol, SymbolTable* table) {
    for (int i = 0; i < table->len; i++) {
        if (!strcmp(table->pairs[i]->symbol, symbol)) {
            return table->pairs[i]->rom_address;
        }
    }
    // -1 is sentinel value that means "symbol not found".
    // this works well enough, since -1 is not a valid ROM address
    return -1;
}


int add_symbol_to_table(char *symbol, int rom_address, SymbolTable* table) {
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

    SymbolAddressPair *new_pair_ptr = new_pair(symbol, rom_address);
    table->pairs[table->len - 1] = new_pair_ptr;

    return 0;
}
