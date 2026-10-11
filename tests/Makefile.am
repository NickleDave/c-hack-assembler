AM_LDFLAGS  = $(CRITERION_LIBS)
AM_CFLAGS 	= $(CRITERION_CFLAGS)

check_PROGRAMS = \
    test_code \
    test_parser \
    test_symbol_table

test_code_SOURCES = test_code.c cr_helpers.c $(top_builddir)/src/code.c $(top_builddir)/src/code.h
test_parser_SOURCES = test_parser.c cr_helpers.c $(top_builddir)/src/parser.c $(top_builddir)/src/parser.h
test_symbol_table_SOURCES = test_symbol_table.c cr_helpers.c $(top_builddir)/src/symbol_table.c $(top_builddir)/src/symbol_table.h


TESTS = $(check_PROGRAMS)