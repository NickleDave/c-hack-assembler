#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol_table.h"


struct SymbolAddressPair* new_symbol_table() {
    struct SymbolAddressPair* ptr = NULL;
}


bool is_symbol_in_table(char * symbol, struct SymbolAddressPair* table) {
    if (table == NULL) {
        return false;
    }
    size_t table_len = (sizeof(table)) / sizeof(struct SymbolAddressPair);
    for (int i = 0; i<table_len; i++) {
        if (!strcmp(table[i].symbol, symbol)) {
            return true;
        }
    }
    return false;
}


struct SymbolAddressPair* add_symbol_to_table(char *symbol, int rom_address, struct SymbolAddressPair* table) {
    size_t table_len = (sizeof(table)) / sizeof(struct SymbolAddressPair);
    size_t new_len = table_len + 1;
    struct SymbolAddressPair* new_table_ptr = realloc(
        table, sizeof(struct SymbolAddressPair) * new_len
    );
    if (new_table_ptr == NULL) {
        fprintf(stderr, "symbol_table::add_symbol_to_table: realloc of table failed");
        exit(EXIT_FAILURE);
    }
    table = new_table_ptr;
    struct SymbolAddressPair new_pair = {.symbol=symbol, .rom_address=rom_address};
    table[new_len - 1] = new_pair;
    return table;
}
