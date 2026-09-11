#include <stdio.h>

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>

#include "../src/parser.h"

// helper used with all functions that have string parameters
char *cr_strdup(const char *str)
{
    char *ptr = cr_malloc(strlen(str) + 1);
    if (ptr)
        strcpy(ptr, str);
    return ptr;
}


// ---- test `remove_spaces` ----------------------------------------------------------------------------------------------------
struct RemoveSpacesParameters{
    char* str_with_spaces;
    char* returned_str;
    char* expected_str;
};


void free_RemoveSpacesParameters(struct criterion_test_params *crp)
{
    for (size_t i = 0; i < crp->length; ++i) {
        struct RemoveSpacesParameters *params = (struct RemoveSpacesParameters *) crp->params + i;
        cr_free(params->str_with_spaces);
        cr_free(params->returned_str);
        cr_free(params->expected_str);
    }
    cr_free(crp->params);
}


ParameterizedTestParameters(test_parser, test_remove_spaces){
    int nb_tuples = 1;
    struct RemoveSpacesParameters *params = cr_malloc(sizeof (struct RemoveSpacesParameters) * nb_tuples);

    char * expected_str = cr_strdup("abc");
    params[0] = (struct RemoveSpacesParameters) {
        .str_with_spaces=cr_strdup(" a b c "), 
        .returned_str=cr_strdup("     "), 
        .expected_str=cr_strdup("abc")
    };

    return cr_make_param_array(
        struct RemoveSpacesParameters, params, nb_tuples, free_RemoveSpacesParameters
    );
}


ParameterizedTest(struct RemoveSpacesParameters *params, test_parser, test_remove_spaces)
{
    remove_spaces(params->returned_str, params->str_with_spaces);
    cr_assert(eq(str, params->expected_str, params->returned_str));
}


// ---- test `is_all_digits` --------------------------------------------------------------------------------------------------
//  `IsSomething` because we use the same struct + `free` function for two tests:
// the test of `is_all_digits` and the test of `is_valid_symbol`
struct IsSomething {
    char * str;
    bool expected_return_value;
};


void free_IsSomething(struct criterion_test_params *crp)
{
    cr_free(crp->params);
}


ParameterizedTestParameters(test_parser, test_is_all_digits) {
    int nb_tuples = 4;
    struct IsSomething *params = cr_malloc(sizeof (struct IsSomething) * nb_tuples);

    params[0] = (struct IsSomething) {
        .str=cr_strdup("12345"),
        .expected_return_value=true 
    };
    params[1] = (struct IsSomething) {
        .str=cr_strdup("-12345"),
        // we don't accept negative values
        .expected_return_value=false 
    };
    params[2] = (struct IsSomething) {
        .str=cr_strdup("abcde"),
        // alpha chars should definitely not parse as numeric
        .expected_return_value=false 
    };
    params[3] = (struct IsSomething) {
        .str=cr_strdup("Abcde"),
        // alpha chars should definitely not parse as numeric
        .expected_return_value=false 
    };
    params[4] = (struct IsSomething) {
        .str=cr_strdup("0"),
        .expected_return_value=true 
    };

    return cr_make_param_array(
        struct IsSomething, params, nb_tuples, free_IsSomething
    );

};


ParameterizedTest(struct IsSomething *params, test_parser, test_is_all_digits)
{
    bool returned_value = is_all_digits(params->str);
    // we could make `eq` work with bool: https://github.com/Snaipe/Criterion/issues/593
    // but that requires abusing the facilities for user-defined types; feels like overkill
    cr_assert(eq(int, params->expected_return_value, returned_value));
}


// ---- test `constant_str_to_int` -------------------------------------------------------------------------------------------
struct ConstantStrToInt {
    char * str;
    int expected_return_value;
};


void free_ConstantStrToInt(struct criterion_test_params *crp)
{
    cr_free(crp->params);
}


ParameterizedTestParameters(test_parser, test_constant_str_to_int) {
    int nb_tuples = 5;
    struct ConstantStrToInt *params = cr_malloc(sizeof (struct ConstantStrToInt) * nb_tuples);

    params[0] = (struct ConstantStrToInt) {
        .str=cr_strdup("12345"),
        .expected_return_value=12345 
    };
    params[1] = (struct ConstantStrToInt) {
        .str=cr_strdup("0"),
        .expected_return_value=0 
    };
    params[2] = (struct ConstantStrToInt) {
        .str=cr_strdup("32767"),
        .expected_return_value=32767
    };
    // it's logically impossible for `parse_line` to pass a negative constant
    // since that would return false when we call `is_all_digits`
    // but still we should test that the function behaves as expected and returns -1
    params[3] = (struct ConstantStrToInt) {
        .str=cr_strdup("-256"),
        .expected_return_value=-1
    };
    params[4] = (struct ConstantStrToInt) {
        .str=cr_strdup("32769"),
        .expected_return_value=-1
    };

    return cr_make_param_array(
        struct ConstantStrToInt, params, nb_tuples, free_ConstantStrToInt
    );
};


ParameterizedTest(struct ConstantStrToInt *params, test_parser, test_constant_str_to_int)
{
    int returned_value = constant_str_to_int(params->str);
    cr_assert(eq(int, params->expected_return_value, returned_value));
}


// ---- test `is_valid_symbol` --------------------------------------------------------------------------------------------------
ParameterizedTestParameters(test_parser, test_is_valid_symbol) {
    int nb_tuples = 6;
    struct IsSomething *params = cr_malloc(sizeof (struct IsSomething) * nb_tuples);

    params[0] = (struct IsSomething) {
        .str=cr_strdup("1Abcde"),
        .expected_return_value=false
    };
    params[1] = (struct IsSomething) {
        .str=cr_strdup("-Abcde"),
        .expected_return_value=false 
    };
    params[2] = (struct IsSomething) {
        .str=cr_strdup("A"),
        .expected_return_value=true 
    };
    params[3] = (struct IsSomething) {
        .str=cr_strdup("Abcde"),
        .expected_return_value=true 
    };
    params[4] = (struct IsSomething) {
        .str=cr_strdup(":Symbol"),
        .expected_return_value=true 
    };
    params[5] = (struct IsSomething) {
        .str=cr_strdup("Symb_bol123"),
        .expected_return_value=true 
    };

    return cr_make_param_array(
        struct IsSomething, params, nb_tuples, free_IsSomething
    );
};



// ---- test `char_count` ----------------------------------------------------------------------------------------------------
struct CharCount {
    char * str;
    char char_;
    int expected_return_value;
};


void free_CharCount(struct criterion_test_params *crp)
{
    cr_free(crp->params);
}


ParameterizedTestParameters(test_parser, test_char_count) {
    int nb_tuples = 5;
    struct CharCount *params = cr_malloc(sizeof (struct CharCount) * nb_tuples);

    params[0] = (struct CharCount) {
        .str=cr_strdup("M=D"),
        .char_='=',
        .expected_return_value=1 
    };
    params[1] = (struct CharCount) {
        .str=cr_strdup("M=D;JMP"),
        .char_=';',
        .expected_return_value=1 
    };
    params[2] = (struct CharCount) {
        .str=cr_strdup("M==D"),
        .char_='=',
        .expected_return_value=2
    };
    params[3] = (struct CharCount) {
        .str=cr_strdup("M=D;;JMP"),
        .char_=';',
        .expected_return_value=2
    };
    params[4] = (struct CharCount) {
        .str=cr_strdup(";JMP"),
        .char_='=',
        .expected_return_value=0
    };

    return cr_make_param_array(
        struct ConstantStrToInt, params, nb_tuples, free_ConstantStrToInt
    );
};


ParameterizedTest(struct CharCount *params, test_parser, test_char_count)
{
    int returned_value = char_count(params->str, params->char_);
    cr_assert(eq(int, params->expected_return_value, returned_value));
}


ParameterizedTest(struct IsSomething *params, test_parser, test_is_valid_symbol)
{
    bool returned_value = is_valid_symbol(params->str);
    // we could make `eq` work with bool: https://github.com/Snaipe/Criterion/issues/593
    // but that requires abusing the facilities for user-defined types; feels like overkill
    cr_assert(eq(int, params->expected_return_value, returned_value));
}


// ---- helpers for testing `Command` -----------------------------------------------------------------------------
int cr_user_Command_eq(struct Command *a, struct Command *b)
{
    bool are_equal;
    if (a->symbol == NULL || b->symbol == NULL) {
        are_equal = (
            a->command_type == b->command_type &&
            // next line: `!strcmp` (with exclamation mark) 
            // because it returns 0 when strings match
            a->symbol == b->symbol &&
            a->constant == b->constant &&
            a->dest == b->dest &&
            a->comp == b->comp &&
            a->jump == b->jump
        );
    }
    else {
        are_equal = (
            a->command_type == b->command_type &&
            // next line: `!strcmp` (with exclamation mark) 
            // because it returns 0 when strings match
            !strcmp(a->symbol, b->symbol) &&
            a->constant == b->constant &&
            a->dest == b->dest &&
            a->comp == b->comp &&
            a->jump == b->jump
        );

    }
    return are_equal;
}

char *cr_user_Command_tostr(struct Command *d)
{
    char *out;

    cr_asprintf(
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

// ---- test `parse_line` -------------------------------------------------------------------------------------------------------
Test(test_parser, test_parse_empty_line)
{
    char line[] = "\n\0";
    struct Command expected_command = {EMPTY_LINE};
    struct Command returned_command = parse_line(line);
    cr_assert(eq(type(struct Command), expected_command, returned_command));
}

// ---- test `parse_line` with a comment -----------------------------------------------------------------------------------------------
Test(test_parser, test_parse_comment)
{
    char line[] = "//this is a comment \0";
    struct Command expected_command = {COMMENT};
    struct Command returned_command = parse_line(line);
    cr_assert(eq(type(struct Command), expected_command, returned_command));
}

Test(test_parser, test_parse_comment_with_whitespace)
{
    char line[] = "  //this is a comment \0";
    struct Command expected_command = {COMMENT};
    struct Command returned_command = parse_line(line);
    cr_assert(eq(type(struct Command), expected_command, returned_command));
}


Test(test_parser, test_parse_empty_line_with_whitespace)
{
    char line[] = "   ";
    struct Command expected_command = {EMPTY_LINE};
    struct Command returned_command = parse_line(line);
    cr_assert(eq(type(struct Command), expected_command, returned_command));
}


struct ParseLineParameters{
    char* line;
    struct Command expected_command;
};


void free_ParseLineParameters(struct criterion_test_params *crp) {
    for (size_t i = 0; i < crp->length; ++i) {
        struct ParseLineParameters * params = (struct ParseLineParameters *) crp->params + i;
        cr_free(params->line);
    }
    cr_free(crp->params);
}


ParameterizedTestParameters(test_parser, test_a_command) {
    int nb_tuples = 6;
    struct ParseLineParameters *params = cr_malloc(sizeof (struct ParseLineParameters) * nb_tuples);

    params[0] = (struct ParseLineParameters) {
        .line=cr_strdup("@Abcde"),
        .expected_command=(struct Command) {A_COMMAND, .symbol=cr_strdup("Abcde")}
    };
    params[1] = (struct ParseLineParameters) {
        .line=cr_strdup("@A"),
        .expected_command=(struct Command) {A_COMMAND, .symbol=cr_strdup("A")}
    };
    params[2] = (struct ParseLineParameters) {
        .line=cr_strdup("@:Symbol"),
        .expected_command=(struct Command) {A_COMMAND, .symbol=cr_strdup(":Symbol")}
    };
    params[3] = (struct ParseLineParameters) {
        .line=cr_strdup("@12345"),
        .expected_command=(struct Command) {A_COMMAND, .constant=12345}
    };
    params[4] = (struct ParseLineParameters) {
        .line=cr_strdup("@0"),
        .expected_command=(struct Command) {A_COMMAND, .constant=0}
    };
    params[5] = (struct ParseLineParameters) {
        .line=cr_strdup("@32767"),
        .expected_command=(struct Command) {A_COMMAND, .constant=32767}
    };

    return cr_make_param_array(
        struct ParseLineParameters, params, nb_tuples, free_ParseLineParameters
    );
};


ParameterizedTest(struct ParseLineParameters *params, test_parser, test_a_command)
{
    struct Command returned_command = parse_line(params->line);
    cr_assert(eq(type(struct Command), params->expected_command, returned_command));
}


ParameterizedTestParameters(test_parser, test_l_command) {
    int nb_tuples = 3;
    struct ParseLineParameters *params = cr_malloc(sizeof (struct ParseLineParameters) * nb_tuples);

    params[0] = (struct ParseLineParameters) {
        .line=cr_strdup("(Abcde)"),
        .expected_command=(struct Command) {L_COMMAND, .symbol=cr_strdup("Abcde")}
    };
    params[1] = (struct ParseLineParameters) {
        .line=cr_strdup("(A)"),
        .expected_command=(struct Command) {L_COMMAND, .symbol=cr_strdup("A")}
    };
    params[2] = (struct ParseLineParameters) {
        .line=cr_strdup("(:Symbol)"),
        .expected_command=(struct Command) {L_COMMAND, .symbol=cr_strdup(":Symbol")}
    };

    return cr_make_param_array(
        struct ParseLineParameters, params, nb_tuples, free_ParseLineParameters
    );
};


ParameterizedTest(struct ParseLineParameters *params, test_parser, test_l_command)
{
    struct Command returned_command = parse_line(params->line);
    cr_assert(eq(type(struct Command), params->expected_command, returned_command));
}

