#include "getopt_dos.h"

static int str_case_cmp_len(const char* s1, const char* s2, unsigned int len)
{
	for(unsigned int i = 0; i < len; i++)
	{
		int c1 = tolower(s1[i]);
		int c2 = tolower(s2[i]);
		if(c1 < c2) return -1;
		if(c1 > c2) return  1;
		if(c1 == '\0' && c1 == c2) return 0;
	}
	return 0;
}

int getopt_dos_create_context(gdos_context *ctx, int argc, const char **argv, const gdos_option *opts)
{
    ctx->argc = argc;
    ctx->argv = argv;
    ctx->opts = opts;
    ctx->optind = 1;

	/* reset opt-arg */
    memset( &(ctx->current_opt_arg), 0, sizeof(gdos_argument));

    /* count options */
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
	/* are all options processed? */
	if(ctx->optind == ctx->argc || ctx->argc <= 1 )
		return GDOS_NEXT_ALL_OPTIONS_PROCEEDED;

	/* reset opt-arg */
    memset( &(ctx->current_opt_arg), 0, sizeof(gdos_argument));

    const char *current_token = ctx->argv[ctx->optind];

	/* this token has been processed! */
    ctx->optind++;

    if (current_token[0] && current_token[0] != '/') {
		return GDOS_NEXT_NOT_OPTION;
    }

    const gdos_option *found_opt = NULL;

    for (int i=0; i<ctx->num_opts; i++) {
        const gdos_option *opt = ctx->opts + i;
        unsigned int name_len = (unsigned int)strlen(opt->name);

        /* remove the leading slash */
        const char *option_name = current_token + 1;
        if (str_case_cmp_len(opt->name, option_name, name_len) == 0) {
            ctx->arg_type = NO_ARGUMENT;
            return opt->val;
        }
    }

    return GDOS_NEXT_NO_MATCHING_OPTION;
}