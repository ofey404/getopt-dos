#include "greatest.h"
#include "getopt_dos.h"
#include <assert.h>
#include <stdbool.h>

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))

enum
{
    GDOS_TEST_OPTION_A,
    GDOS_TEST_OPTION_XYZ,
    GDOS_TEST_OPTION_C,
    GDOS_TEST_OPTION_C_RST,
    GDOS_TEST_OPTION_C_NO_ARG,
    GDOS_TEST_OPTION_S,
    GDOS_TEST_OPTION_S_XYZ,
    GDOS_TEST_OPTION_S_NO_ARG,
};

static const gdos_option option_list[] =
    {
        {"a", NO_ARGUMENT, GDOS_TEST_OPTION_A},
        {"xyz", NO_ARGUMENT, GDOS_TEST_OPTION_XYZ},
        {"c", COLON_SEPERATED_ARGUMENT, GDOS_TEST_OPTION_C},               /* /c:para_c */
        {"c_rst", COLON_SEPERATED_ARGUMENT, GDOS_TEST_OPTION_C_RST},       /* /c_rst:para_rst */
        {"c_no_arg", COLON_SEPERATED_ARGUMENT, GDOS_TEST_OPTION_C_NO_ARG}, /* /c_no_arg: */
        {"s", SPACE_SEPERATED_ARGUMENT_LIST, GDOS_TEST_OPTION_S},
        {"s_xyz", SPACE_SEPERATED_ARGUMENT_LIST, GDOS_TEST_OPTION_S_XYZ},
        {"s_no_arg", SPACE_SEPERATED_ARGUMENT_LIST, GDOS_TEST_OPTION_S_NO_ARG},
        GDOS_OPTIONS_END};

void print_context(int opt, gdos_context *ctx)
{
    printf("opt = %i\n", opt);
    printf("optind = %d\n", ctx->optind);
    printf("argv[optind-1] = %s\n", ctx->argv[ctx->optind - 1]);
    printf("arg_type = %i\n", ctx->arg_type);
    printf("current_opt_arg = %s\n", ctx->current_opt_arg.single);
    return;
}

int simple_without_argument(int argc, const char *argv[])
{
    bool got_a = false;
    bool got_xyz = false;

    gdos_context ctx;
    ASSERT_EQ(0, getopt_dos_create_context(&ctx, argc, argv, option_list));

    int opt;
    char error_message[80];
    while ((opt = getopt_dos_next(&ctx)) != -1)
    {
        switch (opt)
        {
        case GDOS_TEST_OPTION_A:
            ASSERT(!got_a);
            ASSERT(ctx.arg_type == NO_ARGUMENT);
            got_a = true;
            break;
        case GDOS_TEST_OPTION_XYZ:
            ASSERT(!got_xyz);
            ASSERT(ctx.arg_type == NO_ARGUMENT);
            got_xyz = true;
            break;
        default:
            printf("opt = %i\n", opt);
            FAILm("got an unexpected opt!");
            break;
        }
    }

    ASSERT(got_a);
    ASSERT(got_xyz);
    return 0;
}

int simple_colon_seperated_argument(int argc, const char *argv[])
{
    bool got_c = false;
    bool got_c_rst = false;
    bool got_c_no_arg = false;

    gdos_context ctx;
    ASSERT_EQ(0, getopt_dos_create_context(&ctx, argc, argv, option_list));

    int opt;
    char error_message[80];
    while ((opt = getopt_dos_next(&ctx)) != -1)
    {
        switch (opt)
        {
        case GDOS_TEST_OPTION_C:
            ASSERT(!got_c);
            ASSERT(ctx.arg_type == COLON_SEPERATED_ARGUMENT);
            ASSERT_STR_EQ("para_c", ctx.current_opt_arg.single);
            got_c = true;
            break;
        case GDOS_TEST_OPTION_C_RST:
            ASSERT(!got_c_rst);
            ASSERT(ctx.arg_type == COLON_SEPERATED_ARGUMENT);
            ASSERT_STR_EQ("para_rst", ctx.current_opt_arg.single);
            got_c_rst = true;
            break;
        case GDOS_TEST_OPTION_C_NO_ARG:
            ASSERT(!got_c_no_arg);
            ASSERT(ctx.arg_type == COLON_SEPERATED_ARGUMENT);
            ASSERT_STR_EQ("", ctx.current_opt_arg.single);
            got_c_no_arg = true;
            break;
        default:
            print_context(opt, &ctx);
            FAILm("got an unexpected opt!");
            break;
        }
    }

    ASSERT(got_c);
    ASSERT(got_c_rst);
    ASSERT(got_c_no_arg);
    return 0;
    return 0;
}

int argument_list_eq(gdos_arglist *lhs, gdos_arglist *rhs) {
    if (lhs->num_args != rhs->num_args) {
        return -1;
    }
    for (int i=0; i<lhs->num_args; i++) {
        if (strcmp(lhs->argument_list[i], rhs->argument_list[i]) != 0) {
            return -1;
        }
    }
    return 0;
}

int simple_space_seperated_argument_list(int argc, const char *argv[]) {
    bool got_s = false;
    const char *arglist[] = {"para1", "para2"};
    gdos_arglist s_arg = {
        .argument_list = arglist,
        .num_args = 2
    };
    bool got_s_xyz = false;
    gdos_arglist s_xyz_arg = {
        .argument_list = arglist,
        .num_args = 2
    };
    bool got_s_no_arg = false;
    gdos_arglist s_no_arg = {
        NULL,
        0
    };

    gdos_context ctx;
    ASSERT_EQ(0, getopt_dos_create_context(&ctx, argc, argv, option_list));

    int opt;
    char error_message[80];
    while ((opt = getopt_dos_next(&ctx)) != -1)
    {
        switch (opt)
        {
        case GDOS_TEST_OPTION_S:
            ASSERT(!got_s);
            ASSERT(ctx.arg_type == SPACE_SEPERATED_ARGUMENT_LIST);
            ASSERT_EQ(0, argument_list_eq(&s_arg, &ctx.current_opt_arg.list));
            got_s = true;
            break;
        case GDOS_TEST_OPTION_S_XYZ:
            ASSERT(!got_s_xyz);
            ASSERT(ctx.arg_type == SPACE_SEPERATED_ARGUMENT_LIST);
            ASSERT_EQ(0, argument_list_eq(&s_xyz_arg, &ctx.current_opt_arg.list));
            got_s_xyz = true;
            break;
        case GDOS_TEST_OPTION_S_NO_ARG:
            ASSERT(!got_s_no_arg);
            ASSERT(ctx.arg_type == SPACE_SEPERATED_ARGUMENT_LIST);
            ASSERT_EQ(0, argument_list_eq(&s_no_arg, &ctx.current_opt_arg.list));
            got_s_no_arg = true;
            break;
        default:
            print_context(opt, &ctx);
            FAILm("got an unexpected opt!");
            break;
        }
    }

    ASSERT(got_s);
    ASSERT(got_s_xyz);
    ASSERT(got_s_no_arg);
    return 0;
    return 0;
}

/* test functions begin here */

TEST nothing()
{
    const char *argv[] = {"dummy_program"};
    int argc = (int)(sizeof(argv) / sizeof(argv[0]));

    gdos_context ctx;
    ASSERT_EQ(0, getopt_dos_create_context(&ctx, argc, argv, option_list));
    ASSERT_EQ(-1, getopt_dos_next(&ctx));
    return 0;
}

/* test return value on failure */
TEST failed()
{
    return 0;
}

/* also test case insensitivity */
TEST without_argument()
{
    const char *argv1[] = {"dummy_program", "/a", "/xyz"};
    const char *argv2[] = {"dummy_program", "/A", "/XYZ"};

    if (simple_without_argument((int)ARRAY_LENGTH(argv1), argv1) != 0)
    {
        return -1;
    }
    if (simple_without_argument((int)ARRAY_LENGTH(argv2), argv2) != 0)
    {
        return -1;
    }
    return 0;
}

TEST colon_seperated_argument()
{
    const char *argv1[] = {"dummy_program", "/c:para_c", "/c_rst:para_rst", "/c_no_arg:"};
    const char *argv2[] = {"dummy_program", "/C:para_c", "/C_RST:para_rst", "/C_NO_ARG:"};
    if (simple_colon_seperated_argument((int)ARRAY_LENGTH(argv1), argv1) != 0)
    {
        return -1;
    }
    if (simple_colon_seperated_argument((int)ARRAY_LENGTH(argv2), argv2) != 0)
    {
        return -1;
    }
}
TEST space_seperated_argument_list()
{
    const char *argv1[] = {"dummy_program", "/s", "para1", "para2", "/s_xyz", "para1", "para2", "/s_no_arg"};
    const char *argv2[] = {"dummy_program", "/S", "para1", "para2", "/S_XYZ", "para1", "para2", "/S_NO_ARG"};
    
    if (simple_space_seperated_argument_list((int)ARRAY_LENGTH(argv1), argv1) != 0)
    {
        return -1;
    }
    if (simple_space_seperated_argument_list((int)ARRAY_LENGTH(argv2), argv2) != 0)
    {
        return -1;
    }
}

GREATEST_SUITE(getopt_dos)
{
    RUN_TEST(nothing);
    RUN_TEST(without_argument);
    RUN_TEST(colon_seperated_argument);
    RUN_TEST(space_seperated_argument_list);

}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(getopt_dos);
    GREATEST_MAIN_END();
}
