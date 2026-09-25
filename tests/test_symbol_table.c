#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>

#include "cr_helpers.h"

#include "../src/symbol_table.h"


Test(test_symbol_table, test_new_symbol_table) {
    SymbolTable * table = new_symbol_table();
    cr_assert(table->pairs == NULL);
    cr_assert(table->len == 0);
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
    char * symbol;
    int address;
    int expected_success;
    SymbolAddressPair *expected_pair;
};


void free_test_cases(struct criterion_test_params *crp)
{
    for (size_t i=0; i<crp->length; ++i) {
        struct AddToTableCase *test_case = (struct AddToTableCase *) crp->params + i;
        cr_free(test_case->symbol);
        cr_free(test_case->expected_pair);
    }
    cr_free(crp->params);
}


ParameterizedTestParameters(test_code, test_add_symbol_to_table) {
    int num_test_cases = 4;
    struct AddToTableCase *test_cases = cr_malloc(sizeof(struct AddToTableCase) * num_test_cases);

    char *symbol0 = cr_strdup("NewSymbol");
    SymbolAddressPair *pair0 = cr_malloc(sizeof(SymbolAddressPair) + sizeof(symbol0));
    memcpy(pair0->symbol, symbol0, sizeof(symbol0));
    pair0->rom_address = 15;
    test_cases[0] = (struct AddToTableCase) {
        .symbol=symbol0,
        .address=pair0->rom_address,
        .expected_success=0,
        .expected_pair=pair0
    };

    char *symbol1 = cr_strdup("loop2");
    SymbolAddressPair *pair1 = cr_malloc(sizeof(SymbolAddressPair) + sizeof(symbol1));
    memcpy(pair1->symbol, symbol1, sizeof(symbol1));
    pair1->rom_address = 20;
    test_cases[1] = (struct AddToTableCase) {
        .symbol=symbol1,
        .address=pair1->rom_address,
        .expected_success=0,
        .expected_pair=pair1
    };

    char *symbol2 = cr_strdup("sum");
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
    char *symbol3 = cr_strdup("end");
    SymbolAddressPair *pair3 = malloc(sizeof(SymbolAddressPair) + sizeof(symbol3));
    memcpy(pair3->symbol, symbol3, sizeof(symbol3));
    pair3->rom_address = 40;
    test_cases[3] = (struct AddToTableCase) {
        .symbol=symbol3,
        .address=pair3->rom_address,
        .expected_success=-1,
        .expected_pair=pair3
    };

    return cr_make_param_array(
        struct AddToTableCase, test_cases, num_test_cases, free_test_cases
    );
}


ParameterizedTest(
    struct AddToTableCase *test_case, 
    test_code, test_add_symbol_to_table,
    .init=setup_table, .fini=teardown_table
) {
    int success = add_symbol_to_table(test_case->symbol, test_case->address, table);
    cr_assert(eq(success, test_case->expected_success));
    int in_table = 0;
    if (test_case->expected_success == 0) {
        for (int i=0; i < table->len; i++) {
            SymbolAddressPair *pair = table->pairs[i];
            if (
                !strcmp(test_case->expected_pair->symbol, pair->symbol) && 
                test_case->expected_pair->rom_address == pair->rom_address 
            ) in_table = 1;
        }
        cr_assert(eq(in_table, 1));
    }
}
