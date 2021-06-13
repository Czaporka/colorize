#include <sstream>

#include "lib/catch2/catch_amalgamated.hpp"

#include "../src/args.hpp"


TEST_CASE("Args::get_flag", "[Args]")
{
    std::vector<option> options = {
        {"rabbits", no_argument, NULL, 128},
        {"bears", no_argument, NULL, 129},
        {0, 0, 0, 0},
    };
    std::stringstream ss;
    Args::Builder builder(options);

    builder.add_flag("rabbits");
    builder.add_flag("rabbits");
    Args args = builder.build();

    REQUIRE(args.get_flag("rabbits") == 2);
    REQUIRE(args.get_flag("bears") == 0);
    REQUIRE(args.get_flag("foxes") == -1);
}


TEST_CASE("Args::operator<< - w/o args", "[Args]")
{
    std::vector<option> options = {
        {"rabbits", no_argument, NULL, 128},
        {"cats", optional_argument, NULL, 129},
        {0, 0, 0, 0},
    };
    std::stringstream ss;
    Args::Builder builder(options);
    Args args = builder.build();

    ss << args;

    REQUIRE(ss.str() == "Args(flags={cats=0, rabbits=0}, options={}, positionals=[])");
}


TEST_CASE("Args::operator<< - w/ args", "[Args]")
{
    std::vector<option> options = {
        {"rabbits", no_argument, NULL, 128},
        {"cats", optional_argument, NULL, 129},
        {0, 0, 0, 0},
    };
    std::stringstream ss;
    Args::Builder builder(options);
    builder.add_flag("rabbits");
    builder.add_option("cats", "dogs");
    builder.add_positional("ducks");
    Args args = builder.build();

    ss << args;

    REQUIRE(ss.str() == "Args(flags={cats=0, rabbits=1}, options={cats=\"dogs\"}, positionals=[\"ducks\"])");
}
