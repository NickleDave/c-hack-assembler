#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>

#include "parser.h"

/* 
helper to remove whitespace
*/
// Source - https://stackoverflow.com/a/30372683
// Posted by Lundin, modified by community. See post 'Timeline' for change history
// Retrieved 2026-08-21, License - CC BY-SA 3.0
void remove_spaces(char str_trimmed[], char str_untrimmed[])
{
  while (*str_untrimmed != '\0')
  {
    if(!isspace(*str_untrimmed))
    {
      *str_trimmed = *str_untrimmed;
      str_trimmed++;
    }
    str_untrimmed++;
  }
  *str_trimmed = '\0';
}

/* 
helpers to convert constant strings to integers
*/
bool is_all_digits(char str[]) {
    while (*str != '\0') {
        if (!isdigit(*str)) {
            return false;
        }
        str++;
    }
    return true;
}


int constant_str_to_int(char * constant_str) {
    // FIXME should we call `is_all_digits` here again to (re)validate?

    if (strlen(constant_str) > 5) {
        return -1;  // error, can't be greater than 2**15
    }
    char *endptr;
    errno = 0;  // reset before call to strtol
    // since we already know this isn't negative 
    // (because it was accepted by `isnumeric`)
    // and we know it's between 0 and ~99_999, then 
    // we can't have an ERANGE errno on a modern machine
    long val = strtol(constant_str, &endptr, 10);
    int result = (int)val;
    if (result < 0 || result > pow(2, 15) - 1) {
        return -1;
    }
    return result;
}

/* 
helper to get symbol from A-command or C-command
*/
bool is_valid_symbol(char str[]) {
    regex_t regex; // struct that holds compiled regex
    int return_value; // for `regcomp` and `regexec`
    char error_message[100];

    /* 
    first char must be alpha or special; 
    subsequent chars can also be numeric
    */
    char *pattern = "^[a-zA-Z_.$:][a-zA-Z_.$:0-9]*$";

    return_value = regcomp(&regex, pattern, REG_EXTENDED);
    if (return_value != 0) {
        fprintf(stderr, "parser::get_symbol: could not compile regex\n");
        exit(1);
    }

    return_value = regexec(&regex, str, 0, NULL, 0);
    if (return_value == 0) {
        return true;
    } else if (return_value == REG_NOMATCH) {
        return false;
    } else {
        regerror(return_value, &regex, error_message, sizeof(error_message));
        fprintf(
            stderr,
            "Regex match failed: %s\n", 
            error_message
        );
        exit(1);
    }

    regfree(&regex);
}


struct Command parse_line(char line[]) {
    if (strcmp(line, "\n\0") == 0) {
        struct Command command = {EMPTY_LINE};
        return command;
    }

    bool line_had_comment = false;
    char *match = strstr(line, "//");
    if (match != NULL) {
        // "split" the string by replacing the first forward slash with the null terminator
        *match = '\0';
        if (strlen(line) == 0) {
            // this must have been a comment
            struct Command command = {COMMENT};
            return command;
        }
        line_had_comment = true;
    }

    char line_no_whitespace[strlen(line) + 1];
    remove_spaces(line_no_whitespace, line);
    if (strlen(line_no_whitespace) == 0) {
        if (line_had_comment) {
        // this must have been a comment, but with whitespace in front of the "//"
            struct Command command = {COMMENT};
            return command;
        } else {
            struct Command command = {EMPTY_LINE};
            return command;
        }
    }

    if (line_no_whitespace[0] == '@') {
        /* --> A-command, attempt to parse as such */
        // for both `isnumeric` and `get_symbol` we need constant char
        char * a_value = line_no_whitespace + 1;
        if (is_all_digits(a_value)) {
            int constant = constant_str_to_int(a_value);
            if (constant == -1) {
                fprintf(stderr, "parse_line: A-command constant is not a valid value (must be between 0 and 32767):\n%s", line);
                exit(EXIT_FAILURE);
            }
            struct Command command = {A_COMMAND, .constant=constant};
            return command;
        } else {
            if (is_valid_symbol(a_value)) {
                struct Command command = {A_COMMAND, .symbol=a_value};
                return command;
            } else {
                fprintf(
                    stderr,
                    "parser::parse_line: a-value did not match symbol regex: %s",
                    a_value 
                );
                exit(1);
            }
        }
    }
}


void first_pass(FILE *fp) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        struct Command command = parse_line(line);
    }

}
