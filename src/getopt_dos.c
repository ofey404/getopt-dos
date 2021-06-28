#include "getopt_dos.h"


int getopt_dos_create_context(gdos_context *ctx, int argc, const char **argv, const gdos_option *opts)
{
    ctx->argc = argc;
    ctx->argv = argv;
    ctx->opts = opts;
    ctx->optind = 1;

    /* Count options */
    ctx->num_opts = 0;
    const gdos_option *opt = opts;
    while (!(opt->name == 0x0)) {
        ctx->num_opts++;
        opt++;
    }
    return 0;
}

int getopt_dos_next(gdos_context *ctx)
{
    return 0;
}