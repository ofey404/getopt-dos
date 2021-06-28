#include "greatest.h"
#include "getopt_dos.h"
#include <assert.h>
#include <stdbool.h>

enum
{
    GDOS_TEST_OPTION_A,
    GDOS_TEST_OPTION_XYZ,
};

static const gdos_option option_list[] =
    {
        {"a", NO_ARGUMENT, GDOS_TEST_OPTION_A},
        {"xyz", NO_ARGUMENT, GDOS_TEST_OPTION_XYZ},
        GDOS_OPTIONS_END};

TEST nothing()
{
    const char *argv[] = {"dummy_program"};
    int argc = (int)(sizeof(argv) / sizeof(argv[0]));

    gdos_context ctx;
    ASSERT_EQ(0, getopt_dos_create_context(&ctx, argc, argv, option_list));
    ASSERT_EQ(-1, getopt_dos_next(&ctx));
    return 0;
}

TEST simple_without_argument(int argc, const char *argv[])
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

/* Case insensitive */
TEST flags_without_argument() {
    const char *argv1[] = {"dummy_program", "/a", "/xyz"};
    const char *argv2[] = {"dummy_program", "/A", "/XYZ"};

    if (simple_without_argument((int)(sizeof(argv1) / sizeof(argv1[0])), argv1) != 0) {
        return -1;
    }
    if (simple_without_argument((int)(sizeof(argv2) / sizeof(argv2[0])), argv2) != 0) {
        return -1;
    }
    return 0;
}

GREATEST_SUITE(getopt_dos)
{
    RUN_TEST(nothing);
    RUN_TEST(flags_without_argument);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(getopt_dos);
    GREATEST_MAIN_END();
}
