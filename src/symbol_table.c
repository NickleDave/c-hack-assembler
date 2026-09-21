#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol_table.h"


SymbolTable * new_symbol_table(void) {
    SymbolTable * out = malloc(sizeof(SymbolTable));
    *out = (SymbolTable) { };
    return out;
}


void free_symbol_table(SymbolTable * table) {
    free(table);
}


int table_lookup_symbol(char * symbol, SymbolTable* table) {
    for (int i = 0; i < table->len; i++) {
        if (!strcmp(table->pairs[i].symbol, symbol)) {
            return table->pairs[i].rom_address;
        }
    }
    // -1 is sentinel value that means "symbol not found".
    // this works well enough, since -1 is not a valid ROM address
    return -1;
}


int add_symbol_to_table(char *symbol, int rom_address, struct SymbolTable* table) {
    // pre-condition: symbol can't already be in table, return fail if it is
    if (!(table_lookup_symbol(symbol, table) == -1)) {
        return -1;
    }

    // ok, now add to table
    table->len++;
    struct SymbolAddressPair* new_pairs_ptr = realloc(
        table->pairs, sizeof(struct SymbolAddressPair) * table->len
    );
    if (new_pairs_ptr == NULL) {
        fprintf(stderr, "symbol_table::add_symbol_to_table: realloc of table->pairs failed");
        exit(EXIT_FAILURE);
    }
    table->pairs = new_pairs_ptr;
    struct SymbolAddressPair new_pair = {.symbol=symbol, .rom_address=rom_address};
    table->pairs[table->len - 1] = new_pair;
    return 0;
}
