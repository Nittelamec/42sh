# Init the 42sh project
AC_INIT([42sh], [1.0], [42sh@assistants.epita.fr])

# Setup automake
AM_INIT_AUTOMAKE([subdir-objects] [foreign])

# Pretty display of makefile rules
AM_SILENT_RULES([yes])

# Enable ar for Makefile
AM_PROG_AR

# Check if a ranlib is available
AC_PROG_RANLIB

# Check if a compiler is available for c
AC_PROG_CC

# Check if a compiler have this list of  flags
AX_COMPILER_FLAGS([], [], [], [-Wall -Wextra -Werror -Wvla -pedantic -std=c99])

# List Makefile in subdirectories
AC_CONFIG_FILES([
                    Makefile
                    src/Makefile
                    src/parser/Makefile
                    src/ast/Makefile
                    src/lexer/Makefile
                    tests/Makefile
                    tests/functionnal_test/Makefile
                    tests/unit_test/Makefile
                    src/io/Makefile
                    src/builtins/Makefile
                    src/expansion/Makefile
                ])
AC_OUTPUT
