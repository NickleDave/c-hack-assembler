#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/parser.h"


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


int eq_command(struct Command *a, struct Command *b)
{
    /* this monstrosity of a conditional is meant to handle the fact that
    4 of the 6 members of `struct Command` are char arrays that can be NULL,
    and we will get a segfault if we strcmp with NULL;
    the ternary operator is saying "if either of these are NULL, test that 
    they are `==` (both NULL); else test they are equal with `strcmp`"
    */
    return(
        (
            a->symbol == NULL || b->symbol == NULL
        // NOTE in next line that `!strcmp` has an exclamation mark, because it returns 0 when strings match
        ) ? a->command_type == b->command_type : !strcmp(a->symbol, b->symbol) && 
        a->constant == b->constant &&
        (
            a->dest == NULL || b->dest == NULL
        ) ? a->dest == b->dest : !strcmp(a->dest, b->dest) &&
        (
            a->comp == NULL || b->comp == NULL
        ) ? a->comp == b->comp : !strcmp(a->comp, b->comp) &&
        (
            a->jump == NULL || b->jump
        ) ? a->jump == b->jump : strcmp(a->jump, b->jump)
    );
}


char *print_command(struct Command *d)
{
    char *out;

    asprintf(
        &out, 
        "struct Command{.command_type=%d, .symbol=%s, .constant=%d, .dest = %s, .comp = %s, .jump = %s }", 
        d->command_type,
        d->symbol,
        d->constant, 
        d->dest,
        d->comp,
        d->jump
    );
    return out;
}


struct ParseLineParameters{
    char* line;
    struct Command expected_command;
};


void test_command(struct ParseLineParameters params)
{
    struct Command returned_command = parse_line(params.line);
    char * err_msg;
    ASSERT_MSG(
        eq_command(&params.expected_command, &returned_command), err_msg
    );
}


void test_a_command_params() {
    int nb_tuples = 6;
    struct ParseLineParameters params[nb_tuples];

    params[0] = (struct ParseLineParameters) {
        .line="@Abcde",
        .expected_command=(struct Command) {A_COMMAND, .symbol="Abcde"}
    };
    params[1] = (struct ParseLineParameters) {
        .line="@A",
        .expected_command=(struct Command) {A_COMMAND, .symbol="A"}
    };
    params[2] = (struct ParseLineParameters) {
        .line="@:Symbol",
        .expected_command=(struct Command) {A_COMMAND, .symbol=":Symbol"}
    };
    params[3] = (struct ParseLineParameters) {
        .line="@12345",
        .expected_command=(struct Command) {A_COMMAND, .constant=12345}
    };
    params[4] = (struct ParseLineParameters) {
        .line="@0",
        .expected_command=(struct Command) {A_COMMAND, .constant=0}
    };
    params[5] = (struct ParseLineParameters) {
        .line="@32767",
        .expected_command=(struct Command) {A_COMMAND, .constant=32767}
    };

    for (int i = 0; i < nb_tuples; i++) {
        test_command(params[i]);
    }
};


void test_c_command_params() {
    int nb_tuples = 6;
    struct ParseLineParameters params[nb_tuples];

    params[0] = (struct ParseLineParameters) {
        .line="D=A",
        .expected_command=(struct Command) {C_COMMAND, .dest="D", .comp="A"}
    };
    params[1] = (struct ParseLineParameters) {
        .line="D=D+A",
        .expected_command=(struct Command) {C_COMMAND, .dest="D", .comp="D+A"}
    };
    params[2] = (struct ParseLineParameters) {
        .line="M=D",
        .expected_command=(struct Command) {C_COMMAND, .dest="M", .comp="D"}
    };
    params[3] = (struct ParseLineParameters) {
        .line="D;JGT",
        .expected_command=(struct Command) {C_COMMAND, .comp="D", .jump="JGT"}
    };
    params[4] = (struct ParseLineParameters) {
        .line="0;JMP",
        .expected_command=(struct Command) {C_COMMAND, .comp="0", .jump="JMP"}
    };
    params[5] = (struct ParseLineParameters) {
        .line="AM=M-1",
        .expected_command=(struct Command) {C_COMMAND, .dest="AM", .comp="M-1"}
    };

    for (int i = 0; i < nb_tuples; i++) {
        test_command(params[i]);
    }
};


int main() {
    test_a_command_params();
    test_c_command_params();
}