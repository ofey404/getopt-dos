#ifndef __GETOPT_DOS__
#define __GETOPT_DOS__
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define GDOS_OPTIONS_END \
    {                    \
        0, 0, 0          \
    }


#define GDOS_NEXT_ALL_OPTIONS_PROCEEDED -1
/* getopt_dos_next() return this as identifier when the 
   char[] ctx->optind points to doesn't start with slash.
   Same with '+' in getopt(). */
#define GDOS_NEXT_NOT_OPTION -2;

/* Same with '?' in getopt() */
#define GDOS_NEXT_NO_MATCHING_OPTION -3;

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

/* `gdos_argument.arg` are pointer to argv array.  */
typedef struct
{
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
    gdos_argtype arg_type;
    gdos_argument current_opt_arg;
} gdos_context;

int getopt_dos_create_context(gdos_context *ctx, int argc, const char **argv, const gdos_option *opts);
int getopt_dos_next(gdos_context *ctx);

#endif /* __GETOPT_DOS__ */