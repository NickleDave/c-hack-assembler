#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>

#include "cr_helpers.h"

#include "../src/symbol_table.h"


Test(test_symbol_table, test_new_symbol_table_is_null) {
    SymbolTable * table = new_symbol_table();
    cr_assert(table->pairs == NULL);
    cr_assert(table->len == 0);
}


/* fixtures for add_symbol_to_table and table_lookup_symbol */
static SymbolTable * table;


#define LENGTH 4


static void setup_table(void) {
    /* making a `table` "by hand" so our fixtures 
    don't rely on the code under test */
    SymbolTable* tmp_ptr = cr_malloc(sizeof(SymbolTable));
    if (tmp_ptr == NULL) {
        fprintf(stderr, "setup_table: malloc of table failed");
        exit(EXIT_FAILURE);
    }
    table = tmp_ptr;

    SymbolAddressPair pairs[LENGTH] = {
        {.symbol=cr_strdup(":Symbol"), .rom_address=10},
        {.symbol=cr_strdup("loop_"), .rom_address=20},
        {.symbol=cr_strdup("mac"), .rom_address=30},
        {.symbol=cr_strdup("end"), .rom_address=50},
    };

}


static void teardown_table(void) {
    /* free a `table` "by hand" so our fixtures 
    don't rely on the code under test */
    for (int i = 0; i < table->len; i++)
        cr_free(table->pairs[i].symbol);
    cr_free(table->pairs);
    cr_free(table);
    table = NULL;
}


struct AddToTableCase {
    char * symbol;
    int address;
    int expected_success;
    SymbolAddressPair expected_pair;
};


void free_test_cases(struct criterion_test_params *crp)
{
    for (size_t i=0; i<crp->length; ++i) {
        struct AddToTableCase *test_case = (struct AddToTableCase *) crp->params + i;
        cr_free(test_case->symbol);
    }
    cr_free(crp->params);
}


ParameterizedTestParameters(test_code, test_add_symbol_to_table) {
    int num_test_cases = 4;
    struct AddToTableCase *test_cases = cr_malloc(sizeof(struct AddToTableCase) * num_test_cases);

    char * symbol0 = cr_strdup("NewSymbol");
    test_cases[0] = (struct AddToTableCase) {
        .symbol=symbol0,
        .address=15,
        .expected_success=0,
        .expected_pair = (SymbolAddressPair) {.symbol=symbol0, .rom_address=15}
    };

    char * symbol1 = cr_strdup("loop2");
    test_cases[1] = (struct AddToTableCase) {
        .symbol=symbol1,
        .address=20,
        .expected_success=0,
        .expected_pair = (SymbolAddressPair) {.symbol=symbol1, .rom_address=20}
    };

    char * symbol2 = cr_strdup("sum");
    test_cases[1] = (struct AddToTableCase) {
        .symbol=symbol2,
        .address=32,
        .expected_success=0,
        .expected_pair = (SymbolAddressPair) {.symbol=symbol2, .rom_address=28}
    };

    char * symbol3= cr_strdup("end");
    test_cases[3] = (struct AddToTableCase) {
        .symbol=symbol3,
        .address=15,
        .expected_success=-1,
        .expected_pair = (SymbolAddressPair) {.symbol=symbol3, .rom_address=40}
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
            SymbolAddressPair pair = table->pairs[i];
            if (
                test_case->expected_pair.symbol == pair.symbol && 
                test_case->expected_pair.rom_address == pair.rom_address 
            ) in_table = 1;
        }
        cr_assert(eq(in_table, 1));
    }
}
