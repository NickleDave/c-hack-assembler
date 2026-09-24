#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H


typedef struct SymbolAddressPair {
    int rom_address;
    char symbol[];
} SymbolAddressPair;


typedef struct SymbolTable {
    SymbolAddressPair **pairs;
    int len;
} SymbolTable;


SymbolTable *new_symbol_table(void);
void free_symbol_table(SymbolTable * table);
int table_lookup_symbol(char * symbol, SymbolTable* table);
int add_symbol_to_table(char *symbol, int rom_address, SymbolTable* table);

#endif
