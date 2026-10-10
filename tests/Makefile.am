AM_LDFLAGS  = $(CRITERION_LIBS)
AM_CFLAGS 	= $(CRITERION_CFLAGS)

check_PROGRAMS = \
    test_code \
    test_parser \
    test_symbol_table

test_code_SOURCES = test_code.c cr_helpers.c
test_parser_SOURCES = test_parser.c cr_helpers.c
test_symbol_table_SOURCES = test_symbol_table.c cr_helpers.c


TESTS = $(check_PROGRAMS)