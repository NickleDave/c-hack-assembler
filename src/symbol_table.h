#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H


typedef struct SymbolAddressPair {
    char *symbol;
    int address;
} SymbolAddressPair;


typedef struct SymbolTable {
    SymbolAddressPair **pairs;
    int len;
} SymbolTable;


SymbolAddressPair *new_pair(char *symbol, int address);
SymbolTable *new_symbol_table(void);
void free_symbol_table(SymbolTable *table);
int table_lookup_symbol(char *symbol, SymbolTable *table);
int add_symbol_to_table(char *symbol, int address, SymbolTable *table);
SymbolTable *get_predefined_table(void);

#endif
