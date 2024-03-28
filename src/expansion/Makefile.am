lib_LIBRARIES = libexpansion.a

libexpansion_a_SOURCES =  \
  expansion.c \
  expansion.h \
  variable.c \
  variable.h \
  init_table.c \
  init_table.h

libexpansion_a_CPPFLAGS = -I$(top_srcdir)/src

libexpansion_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libexpansion.a
