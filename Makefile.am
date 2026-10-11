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


testsuite_SOURCES = \
    tests/cr_helpers.c \
    tests/test_code.c \
    tests/test_parser.c \
    tests/test_symbol_table.c \
    $(Assembler_SOURCES)

testsuite_CFLAGS = $(CRITERION_CFLAGS)
testsuite_LDFLAGS = $(CRITERION_LIBS)

check_PROGRAMS = testsuite
TESTS = $(check_PROGRAMS)

