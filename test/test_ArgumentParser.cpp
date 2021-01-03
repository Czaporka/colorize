#define CATCH_CONFIG_MAIN
#include "lib/catch2/catch_amalgamated.hpp"

#include "../src/ArgumentParser.hpp"


TEST_CASE("ArgumentParser::parse_args()", "[ArgumentParser]" ) {
    int argc = 4;
    char* argv[] = {(char*)"program",
                    (char*)"--long=VALUE",
                    (char*)"-vvv",
                    (char*)"positional"};

    ArgumentParser parser(argc, argv);
    parser.add_argument("long", 'l', required_argument, "", "METAVAR");
    parser.add_argument("verbose", 'v', no_argument, "");
    Args args = parser.parse_args();

    REQUIRE(args.flags['v'] == 3);
    REQUIRE(args.options['l'] == "VALUE");
    REQUIRE(!args.positionals.empty());
    REQUIRE(args.positionals.back() == "positional");
}
