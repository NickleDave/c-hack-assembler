#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

struct SymbolAddressPair {
    char * symbol;
    int rom_address;
};

struct SymbolAddressPair* new_symbol_table();
bool is_symbol_in_table(char * symbol, struct SymbolAddressPair* table);
struct SymbolAddressPair* add_symbol_to_table(char *symbol, int rom_address, struct SymbolAddressPair* table);

#endif
