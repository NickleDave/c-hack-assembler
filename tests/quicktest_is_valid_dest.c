#define _GNU_SOURCE
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/code.h"


#ifndef NDEBUG
    #define ASSERT_MSG(condition, message) \
        do { \
            if (!(condition)) { \
                fprintf(stderr, "Assertion failed: %s (%s:%d)\n", message, __FILE__, __LINE__); \
                abort(); \
            } \
        } while (0)
#else
    #define ASSERT_MSG(condition, message) ((void)0)
#endif


struct IsValidTestCase {
    char * symbol;
    bool expected_is_valid;
};


void test_is_valid_jump(struct IsValidTestCase *test_case) {
    bool is_valid = is_valid_jump(test_case->symbol);
    bool matched = (is_valid == test_case->expected_is_valid);
    printf(
        "%d", matched
    );
    // char * err_msg;
    // sprintf(err_msg, "test for '%s' failed, returned %d and expected %d", test_case->symbol, is_valid, test_case->expected_is_valid);
    // ASSERT_MSG(
    //     (is_valid == test_case->expected_is_valid), err_msg
    // );
}

void test_is_valid_jump_all() {
    int num_test_cases = 16;
    struct IsValidTestCase test_cases[num_test_cases];

    test_cases[0] = (struct IsValidTestCase) {.symbol="", .expected_is_valid=true};
    test_cases[1] = (struct IsValidTestCase) {.symbol="JGT", .expected_is_valid=true};
    test_cases[2] = (struct IsValidTestCase) {.symbol="JEQ", .expected_is_valid=true};
    test_cases[3] = (struct IsValidTestCase) {.symbol="JGE", .expected_is_valid=true};
    test_cases[4] = (struct IsValidTestCase) {.symbol="JLT", .expected_is_valid=true};
    test_cases[5] = (struct IsValidTestCase) {.symbol="JNE", .expected_is_valid=true};
    test_cases[6] = (struct IsValidTestCase) {.symbol="JLE", .expected_is_valid=true};
    test_cases[7] = (struct IsValidTestCase) {.symbol="JMP", .expected_is_valid=true};

    test_cases[8] = (struct IsValidTestCase) {.symbol=" ", .expected_is_valid=false};
    test_cases[9] = (struct IsValidTestCase) {.symbol="JGR", .expected_is_valid=false};
    test_cases[10] = (struct IsValidTestCase) {.symbol="JRG", .expected_is_valid=false};
    test_cases[11] = (struct IsValidTestCase) {.symbol="JFE", .expected_is_valid=false};
    test_cases[12] = (struct IsValidTestCase) {.symbol="JTL", .expected_is_valid=false};
    test_cases[13] = (struct IsValidTestCase) {.symbol="JEN", .expected_is_valid=false};
    test_cases[14] = (struct IsValidTestCase) {.symbol="JEL", .expected_is_valid=false};
    test_cases[15] = (struct IsValidTestCase) {.symbol="JNP", .expected_is_valid=false};

    for (int i=0; i<num_test_cases; i++) {
        test_is_valid_jump(&test_cases[i]);
    }
}


int main() {
    test_is_valid_jump_all();
}