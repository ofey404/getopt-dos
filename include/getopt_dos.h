#ifndef __GETOPT_DOS__
#define __GETOPT_DOS__
#include <stdio.h>

#define GDOS_OPTIONS_END \
    {                    \
        0, 0, 0          \
    }

typedef enum
{
    NO_ARGUMENT,
    COLON_SEPERATED_ARGUMENT,
    SPACE_SEPERATED_ARGUMENT_LIST
} gdos_argtype;

typedef struct
{
    const char **argument_list;
    int num_args;
} gdos_arglist;

typedef struct
{
    const char *name;
    gdos_argtype arg_type;
    int val;
} gdos_option;

typedef struct
{
    gdos_argtype arg_type;
    union
    {
        const char *single;
        gdos_arglist list;
    } arg;
} gdos_argument;

typedef struct
{
    int argc;
    const char **argv;
    const gdos_option *opts;
    int num_opts;
    int optind;
    gdos_argument current_opt_arg;
} gdos_context;

int getopt_dos_create_context(gdos_context *ctx, int argc, const char **argv, const gdos_option *opts);
int getopt_dos_next(gdos_context *ctx);

#endif /* __GETOPT_DOS__ */