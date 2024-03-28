lib_LIBRARIES = libbuiltins.a

libbuiltins_a_SOURCES =  \
  built_boolean.c \
  built_echo.c \
  builtins.h \
  built_exit.c \
  g_exito.c \
  g_exito.h \
  g_lasto.c \
  g_lasto.h \
  built_unset.c

libbuiltins_a_CPPFLAGS = -I$(top_srcdir)/src

libbuiltins_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libbuiltins.a
