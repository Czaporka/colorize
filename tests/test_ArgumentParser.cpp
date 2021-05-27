#define CATCH_CONFIG_MAIN
#include "lib/catch2/catch_amalgamated.hpp"

#include "../src/ArgumentParser.hpp"


#include <iostream>

TEST_CASE("ArgumentParser::parse_args()", "[ArgumentParser]")
{
    int argc = 4;
    char* argv[] = {(char*)"program",
                    (char*)"--long=VALUE",
                    (char*)"-vvv",
                    (char*)"positional"};
    std::stringstream ss;

    ArgumentParser parser(argc, argv);
    parser.add_argument("long", 'l', required_argument, "", "METAVAR");
    parser.add_argument("verbose", 'v', no_argument, "");
    Args args = parser.parse_args();
    parser.print_usage(ss);

    std::cout << args << std::endl;

    REQUIRE(args.flags["verbose"] == 3);
    REQUIRE(args.flags["nonexistent-flag"] == 0);
    REQUIRE(args.options["long"] == "VALUE");
    REQUIRE(args.options["nonexistent-option"] == "");
    REQUIRE(!args.positionals.empty());
    REQUIRE(args.positionals.back() == "positional");
    REQUIRE(ss.str() == "Usage: program [--help|-h] [--long|-l METAVAR] [--verbose|-v]\n");
}
