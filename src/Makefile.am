bin_PROGRAMS = 42sh
42sh_SOURCES = 42sh.c

42sh_CFLAGS= -std=c99 -Werror -Wall -Wextra -Wvla -pedantic
42sh_CPPFLAGS = -I$(top_srcdir)/src

42sh_LDADD = $(top_builddir)/src/parser/libparser.a \
			$(top_builddir)/src/lexer/liblexer.a \
			$(top_builddir)/src/ast/libast.a \
			$(top_builddir)/src/io/libio.a \
			$(top_builddir)/src/builtins/libbuiltins.a \
			$(top_builddir)/src/expansion/libexpansion.a

SUBDIRS = parser \
		  lexer \
		  ast \
		  io \
		  builtins \
		  expansion
