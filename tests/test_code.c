#include <stdbool.h>

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>

#include "cr_helpers.h"

#include "../src/code.h"

/* 
we use the same struct to test three different functions:
- `is_valid_dest`
- `is_valid_comp`
- `is_valid_jump`
*/
struct IsValidTestCase {
    char * symbol;
    bool expected_is_valid;
};


void free_test_cases(struct criterion_test_params *crp)
{
    for (size_t i=0; i<crp->length; ++i) {
        struct IsValidTestCase *test_case = (struct IsValidTestCase *) crp->params + i;
        cr_free(test_case->symbol);
    }
    cr_free(crp->params);
}


ParameterizedTestParameters(test_code, test_is_valid_dest) {
    int num_test_cases = 16;
    struct IsValidTestCase *test_cases = cr_malloc(sizeof(struct IsValidTestCase) * num_test_cases);

    test_cases[0] = (struct IsValidTestCase) {.symbol=cr_strdup(""), .expected_is_valid=true};
    test_cases[1] = (struct IsValidTestCase) {.symbol=cr_strdup("M"), .expected_is_valid=true};
    test_cases[2] = (struct IsValidTestCase) {.symbol=cr_strdup("D"), .expected_is_valid=true};
    test_cases[3] = (struct IsValidTestCase) {.symbol=cr_strdup("MD"), .expected_is_valid=true};
    test_cases[4] = (struct IsValidTestCase) {.symbol=cr_strdup("A"), .expected_is_valid=true};
    test_cases[5] = (struct IsValidTestCase) {.symbol=cr_strdup("AM"), .expected_is_valid=true};
    test_cases[6] = (struct IsValidTestCase) {.symbol=cr_strdup("AD"), .expected_is_valid=true};
    test_cases[7] = (struct IsValidTestCase) {.symbol=cr_strdup("AMD"), .expected_is_valid=true};

    test_cases[8] = (struct IsValidTestCase) {.symbol=cr_strdup(" "), .expected_is_valid=false};
    test_cases[9] = (struct IsValidTestCase) {.symbol=cr_strdup("N"), .expected_is_valid=false};
    test_cases[10] = (struct IsValidTestCase) {.symbol=cr_strdup("F"), .expected_is_valid=false};
    test_cases[11] = (struct IsValidTestCase) {.symbol=cr_strdup("MDD"), .expected_is_valid=false};
    test_cases[12] = (struct IsValidTestCase) {.symbol=cr_strdup("S"), .expected_is_valid=false};
    test_cases[13] = (struct IsValidTestCase) {.symbol=cr_strdup("SM"), .expected_is_valid=false};
    test_cases[14] = (struct IsValidTestCase) {.symbol=cr_strdup("AF"), .expected_is_valid=false};
    test_cases[15] = (struct IsValidTestCase) {.symbol=cr_strdup("AMDD"), .expected_is_valid=false};

    return cr_make_param_array(
        struct IsValidTestCase, test_cases, num_test_cases, free_test_cases
    );
}


ParameterizedTest(struct IsValidTestCase *test_case, test_code, test_is_valid_dest) {
    bool is_valid = is_valid_dest(test_case->symbol);
    cr_assert(eq(is_valid, test_case->expected_is_valid)); 
}


ParameterizedTestParameters(test_code, test_is_valid_comp) {
    int num_test_cases = 56;
    struct IsValidTestCase *test_cases = cr_malloc(sizeof(struct IsValidTestCase) * num_test_cases);

    test_cases[0] = (struct IsValidTestCase) {.symbol=cr_strdup("0"), .expected_is_valid=true};
    test_cases[1] = (struct IsValidTestCase) {.symbol=cr_strdup("1"), .expected_is_valid=true};
    test_cases[2] = (struct IsValidTestCase) {.symbol=cr_strdup("-1"), .expected_is_valid=true};
    test_cases[3] = (struct IsValidTestCase) {.symbol=cr_strdup("D"), .expected_is_valid=true};
    test_cases[4] = (struct IsValidTestCase) {.symbol=cr_strdup("A"), .expected_is_valid=true};
    test_cases[5] = (struct IsValidTestCase) {.symbol=cr_strdup("!D"), .expected_is_valid=true};
    test_cases[6] = (struct IsValidTestCase) {.symbol=cr_strdup("!A"), .expected_is_valid=true};
    test_cases[7] = (struct IsValidTestCase) {.symbol=cr_strdup("-D"), .expected_is_valid=true};
    test_cases[8] = (struct IsValidTestCase) {.symbol=cr_strdup("-A"), .expected_is_valid=true};
    test_cases[9] = (struct IsValidTestCase) {.symbol=cr_strdup("D+1"), .expected_is_valid=true};
    test_cases[10] = (struct IsValidTestCase) {.symbol=cr_strdup("A+1"), .expected_is_valid=true};
    test_cases[11] = (struct IsValidTestCase) {.symbol=cr_strdup("D-1"), .expected_is_valid=true};
    test_cases[12] = (struct IsValidTestCase) {.symbol=cr_strdup("A-1"), .expected_is_valid=true};
    test_cases[13] = (struct IsValidTestCase) {.symbol=cr_strdup("D+A"), .expected_is_valid=true};
    test_cases[14] = (struct IsValidTestCase) {.symbol=cr_strdup("D-A"), .expected_is_valid=true};
    test_cases[15] = (struct IsValidTestCase) {.symbol=cr_strdup("A-D"), .expected_is_valid=true};
    test_cases[16] = (struct IsValidTestCase) {.symbol=cr_strdup("D&A"), .expected_is_valid=true};
    test_cases[17] = (struct IsValidTestCase) {.symbol=cr_strdup("D|A"), .expected_is_valid=true};
    test_cases[18] = (struct IsValidTestCase) {.symbol=cr_strdup("M"), .expected_is_valid=true};
    test_cases[19] = (struct IsValidTestCase) {.symbol=cr_strdup("!M"), .expected_is_valid=true};
    test_cases[20] = (struct IsValidTestCase) {.symbol=cr_strdup("-M"), .expected_is_valid=true};
    test_cases[21] = (struct IsValidTestCase) {.symbol=cr_strdup("M+1"), .expected_is_valid=true};
    test_cases[22] = (struct IsValidTestCase) {.symbol=cr_strdup("M-1"), .expected_is_valid=true};
    test_cases[23] = (struct IsValidTestCase) {.symbol=cr_strdup("D+M"), .expected_is_valid=true};
    test_cases[24] = (struct IsValidTestCase) {.symbol=cr_strdup("D-M"), .expected_is_valid=true};
    test_cases[25] = (struct IsValidTestCase) {.symbol=cr_strdup("M-D"), .expected_is_valid=true};
    test_cases[26] = (struct IsValidTestCase) {.symbol=cr_strdup("D&M"), .expected_is_valid=true};
    test_cases[27] = (struct IsValidTestCase) {.symbol=cr_strdup("D|M"), .expected_is_valid=true};

    test_cases[28] = (struct IsValidTestCase) {.symbol=cr_strdup(""), .expected_is_valid=false};
    test_cases[29] = (struct IsValidTestCase) {.symbol=cr_strdup("9"), .expected_is_valid=false};
    test_cases[30] = (struct IsValidTestCase) {.symbol=cr_strdup("2"), .expected_is_valid=false};
    test_cases[31] = (struct IsValidTestCase) {.symbol=cr_strdup("-2"), .expected_is_valid=false};
    test_cases[32] = (struct IsValidTestCase) {.symbol=cr_strdup("E"), .expected_is_valid=false};
    test_cases[33] = (struct IsValidTestCase) {.symbol=cr_strdup("S"), .expected_is_valid=false};
    test_cases[34] = (struct IsValidTestCase) {.symbol=cr_strdup("1D"), .expected_is_valid=false};
    test_cases[35] = (struct IsValidTestCase) {.symbol=cr_strdup("1A"), .expected_is_valid=false};
    test_cases[36] = (struct IsValidTestCase) {.symbol=cr_strdup("_D"), .expected_is_valid=false};
    test_cases[37] = (struct IsValidTestCase) {.symbol=cr_strdup("_A"), .expected_is_valid=false};
    test_cases[38] = (struct IsValidTestCase) {.symbol=cr_strdup("D1"), .expected_is_valid=false};
    test_cases[39] = (struct IsValidTestCase) {.symbol=cr_strdup("A1"), .expected_is_valid=false};
    test_cases[40] = (struct IsValidTestCase) {.symbol=cr_strdup("D-"), .expected_is_valid=false};
    test_cases[41] = (struct IsValidTestCase) {.symbol=cr_strdup("DA"), .expected_is_valid=false};
    test_cases[42] = (struct IsValidTestCase) {.symbol=cr_strdup("D_A"), .expected_is_valid=false};
    test_cases[43] = (struct IsValidTestCase) {.symbol=cr_strdup("A_D"), .expected_is_valid=false};
    test_cases[44] = (struct IsValidTestCase) {.symbol=cr_strdup("D7A"), .expected_is_valid=false};
    test_cases[45] = (struct IsValidTestCase) {.symbol=cr_strdup("D\\A"), .expected_is_valid=false};
    test_cases[46] = (struct IsValidTestCase) {.symbol=cr_strdup("N"), .expected_is_valid=false};
    test_cases[47] = (struct IsValidTestCase) {.symbol=cr_strdup("!N"), .expected_is_valid=false};
    test_cases[48] = (struct IsValidTestCase) {.symbol=cr_strdup("_M"), .expected_is_valid=false};
    test_cases[49] = (struct IsValidTestCase) {.symbol=cr_strdup("DM"), .expected_is_valid=false};
    test_cases[50] = (struct IsValidTestCase) {.symbol=cr_strdup("D_M"), .expected_is_valid=false};
    test_cases[51] = (struct IsValidTestCase) {.symbol=cr_strdup("M_D"), .expected_is_valid=false};
    test_cases[52] = (struct IsValidTestCase) {.symbol=cr_strdup("D7M"), .expected_is_valid=false};
    test_cases[53] = (struct IsValidTestCase) {.symbol=cr_strdup("D\\M"), .expected_is_valid=false};
    test_cases[54] = (struct IsValidTestCase) {.symbol=cr_strdup("AMD"), .expected_is_valid=false};
    test_cases[55] = (struct IsValidTestCase) {.symbol=cr_strdup("MDA"), .expected_is_valid=false};
    
    return cr_make_param_array(
        struct IsValidTestCase, test_cases, num_test_cases, free_test_cases
    );
}


ParameterizedTest(struct IsValidTestCase *test_case, test_code, test_is_valid_comp) {
    bool is_valid = is_valid_comp(test_case->symbol);
    cr_assert(eq(is_valid, test_case->expected_is_valid)); 
}


ParameterizedTestParameters(test_code, test_is_valid_jump) {
    int num_test_cases = 16;
    struct IsValidTestCase *test_cases = cr_malloc(sizeof(struct IsValidTestCase) * num_test_cases);

    test_cases[0] = (struct IsValidTestCase) {.symbol=cr_strdup(""), .expected_is_valid=true};
    test_cases[1] = (struct IsValidTestCase) {.symbol=cr_strdup("JGT"), .expected_is_valid=true};
    test_cases[2] = (struct IsValidTestCase) {.symbol=cr_strdup("JEQ"), .expected_is_valid=true};
    test_cases[3] = (struct IsValidTestCase) {.symbol=cr_strdup("JGE"), .expected_is_valid=true};
    test_cases[4] = (struct IsValidTestCase) {.symbol=cr_strdup("JLT"), .expected_is_valid=true};
    test_cases[5] = (struct IsValidTestCase) {.symbol=cr_strdup("JNE"), .expected_is_valid=true};
    test_cases[6] = (struct IsValidTestCase) {.symbol=cr_strdup("JLE"), .expected_is_valid=true};
    test_cases[7] = (struct IsValidTestCase) {.symbol=cr_strdup("JMP"), .expected_is_valid=true};

    test_cases[8] = (struct IsValidTestCase) {.symbol=cr_strdup(" "), .expected_is_valid=false};
    test_cases[9] = (struct IsValidTestCase) {.symbol=cr_strdup("JGR"), .expected_is_valid=false};
    test_cases[10] = (struct IsValidTestCase) {.symbol=cr_strdup("JRG"), .expected_is_valid=false};
    test_cases[11] = (struct IsValidTestCase) {.symbol=cr_strdup("JFE"), .expected_is_valid=false};
    test_cases[12] = (struct IsValidTestCase) {.symbol=cr_strdup("JTL"), .expected_is_valid=false};
    test_cases[13] = (struct IsValidTestCase) {.symbol=cr_strdup("JEN"), .expected_is_valid=false};
    test_cases[14] = (struct IsValidTestCase) {.symbol=cr_strdup("JEL"), .expected_is_valid=false};
    test_cases[15] = (struct IsValidTestCase) {.symbol=cr_strdup("JNP"), .expected_is_valid=false};

    return cr_make_param_array(
        struct IsValidTestCase, test_cases, num_test_cases, free_test_cases
    );
}


ParameterizedTest(struct IsValidTestCase *test_case, test_code, test_is_valid_jump) {
    bool is_valid = is_valid_jump(test_case->symbol);
    cr_assert(eq(is_valid, test_case->expected_is_valid));
}