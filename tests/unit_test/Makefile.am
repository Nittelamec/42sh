check_PROGRAMS = lexer_test

lexer_test_SOURCES = lexer_test.c
lexer_test_CPPFLAGS = -I$(top_srcdir)/src

lexer_test_LDADD = \
	-lcriterion \
	$(top_builddir)/src/lexer/liblexer.a


check-local: lexer_test
	./lexer_test
