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

TEST no_argument()
{
    const char *argv[] = {"dummy_program", "/a", "/xyz"};
    int argc = (int)(sizeof(argv) / sizeof(argv[0]));

    bool got_a = false;
    bool got_xyz = false;

    gdos_context ctx;
    ASSERT_EQ(0, getopt_dos_create_context(&ctx, argc, argv, option_list));

    int opt;
    while ((opt = getopt_dos_next(&ctx)) != -1)
    {
        switch (opt)
        {
        case GDOS_TEST_OPTION_A:
            ASSERT(!got_a);
            got_a = true;
            break;
        case GDOS_TEST_OPTION_XYZ:
            ASSERT(!got_xyz);
            got_xyz = true;
            break;
        default:
            FAILm("got an unexpected opt!");
            break;
        }
    }

    ASSERT(got_a);
    ASSERT(got_xyz);
    return 0;
}

GREATEST_SUITE(getopt_dos)
{
    RUN_TEST(no_argument);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(getopt_dos);
    GREATEST_MAIN_END();
}
