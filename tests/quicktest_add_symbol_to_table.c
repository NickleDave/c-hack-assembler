#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/symbol_table.h"


void test_new_symbol_table() {
    SymbolTable *table = new_symbol_table();
    assert(table->pairs == NULL);
    assert(table->len == 0);
}


/* fixtures for add_symbol_to_table and table_lookup_symbol */
static SymbolTable *table = NULL;


#define LENGTH 4


/* make a `table` "by hand" so our fixtures 
don't rely on the code under test */
static void setup_table(void) {
    char *symbols[] = {
        ":Symbol",
        "loop_",
        "mac",
        "end"
    };
    int addresses[] = {
        10, 20, 30, 50
    };
    SymbolAddressPair **pairs = malloc(sizeof(SymbolAddressPair*) * LENGTH);
    if (pairs == NULL) {
        fprintf(stderr, "setup_table: malloc of pairs failed");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < LENGTH; i++) {
        char *symbol = symbols[i];
        int address = addresses[i];
        SymbolAddressPair *pair = malloc(sizeof(SymbolAddressPair) + sizeof(symbol));
        if (pair == NULL) {
            fprintf(stderr, "setup_table: malloc of pair failed");
            exit(EXIT_FAILURE);
        }
        memcpy(pair->symbol, symbol, sizeof(symbol));
        pair->rom_address = address;
        pairs[i] = pair;
    }

    SymbolTable *tmp_ptr = malloc(sizeof(SymbolTable) + sizeof(pairs));
    if (tmp_ptr == NULL) {
        fprintf(stderr, "setup_table: malloc for table failed");
        exit(EXIT_FAILURE);
    }
    table = tmp_ptr;
    table->len = LENGTH;
    table->pairs = pairs;
}


/* free a `table` "by hand" so our fixtures 
don't rely on the code under test */
static void teardown_table(void) {
    for (int i = 0; i < LENGTH; i++) {
        free(table->pairs[i]);
    }
    free(table->pairs);
    free(table);
    table = NULL;
}


struct AddToTableCase {
    char *symbol;
    int address;
    int expected_success;
    SymbolAddressPair *expected_pair;
};


void test_add_symbol_to_table(struct AddToTableCase *test_case) {
    setup_table();

    int success = add_symbol_to_table(test_case->symbol, test_case->address, table);
    assert(success == test_case->expected_success);
    int in_table = 0;
    if (test_case->expected_success == 0) {
        for (int i=0; i < table->len; i++) {
            SymbolAddressPair *pair = table->pairs[i];
            if (
                !strcmp(test_case->expected_pair->symbol, pair->symbol) && // `!strcmp` because a match returns 0
                test_case->expected_pair->rom_address == pair->rom_address
            ) in_table = 1;
        }
        assert(in_table == 1);
    }

    teardown_table();
}


#define num_test_cases 4


void test_add_symbol_to_table_all_cases(void) {
    
    struct AddToTableCase test_cases[num_test_cases];

    char *symbol0 = "NewSymbol";
    SymbolAddressPair *pair0 = malloc(sizeof(SymbolAddressPair) + sizeof(symbol0));
    memcpy(pair0->symbol, symbol0, sizeof(symbol0));
    pair0->rom_address = 15;
    test_cases[0] = (struct AddToTableCase) {
        .symbol=symbol0,
        .address=pair0->rom_address,
        .expected_success=0,
        .expected_pair=pair0
    };

    char *symbol1 = "loop2";
    SymbolAddressPair *pair1 = malloc(sizeof(SymbolAddressPair) + sizeof(symbol1));
    memcpy(pair1->symbol, symbol1, sizeof(symbol1));
    pair1->rom_address = 20;
    test_cases[1] = (struct AddToTableCase) {
        .symbol=symbol1,
        .address=pair1->rom_address,
        .expected_success=0,
        .expected_pair=pair1
    };

    char *symbol2 = "sum";
    SymbolAddressPair *pair2 = malloc(sizeof(SymbolAddressPair) + sizeof(symbol2));
    memcpy(pair2->symbol, symbol2, sizeof(symbol2));
    pair2->rom_address = 32;
    test_cases[2] = (struct AddToTableCase) {
        .symbol=symbol2,
        .address=pair2->rom_address,
        .expected_success=0,
        .expected_pair=pair2
    };

    // NOTE case we expect to fail because 'end' is already in table
    char *symbol3 = "end";
    SymbolAddressPair *pair3 = malloc(sizeof(SymbolAddressPair) + sizeof(symbol3));
    memcpy(pair3->symbol, symbol3, sizeof(symbol3));
    pair3->rom_address = 40;
    test_cases[3] = (struct AddToTableCase) {
        .symbol=symbol3,
        .address=pair3->rom_address,
        .expected_success=-1,
        .expected_pair=pair3
    };

    for (int i=0; i<num_test_cases; i++) {
        test_add_symbol_to_table(&test_cases[i]);
    }

}

int main(void) {
    test_add_symbol_to_table_all_cases();
}