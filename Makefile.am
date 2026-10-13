bin_PROGRAMS = Assembler
Assembler_SOURCES = \
    src/assemble.c \
    src/assemble.h \
    src/code.c \
    src/code.h \
    src/parser.c \
    src/parser.h \
    src/symbol_table.c \
    src/symbol_table.h \
    src/main.c

check_PROGRAMS = \
    test_code \
    test_parser \
    test_symbol_table

test_code_SOURCES = \
    tests/test_code.c tests/cr_helpers.c src/code.c

test_parser_SOURCES = \
    tests/test_parser.c  tests/cr_helpers.c src/parser.c src/code.c

test_symbol_table_SOURCES = \
    tests/test_symbol_table.c tests/cr_helpers.c src/symbol_table.c

test_code_CFLAGS = $(CRITERION_CFLAGS)
test_code_LDFLAGS = $(CRITERION_LIBS)
test_parser_CFLAGS = $(CRITERION_CFLAGS)
test_parser_LDFLAGS = $(CRITERION_LIBS)
test_symbol_table_CFLAGS = $(CRITERION_CFLAGS)
test_symbol_table_LDFLAGS = $(CRITERION_LIBS)


TESTS = $(check_PROGRAMS)
