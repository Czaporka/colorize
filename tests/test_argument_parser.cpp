#include <sstream>

#include "lib/catch2/catch_amalgamated.hpp"

#include "../src/argument_parser.hpp"


TEST_CASE("ArgumentParser::parse_args", "[ArgumentParser]")
{
    const int argc = 4;
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

    REQUIRE(args.get_flag("verbose") == 3);
    REQUIRE(args.get_flag("nonexistent-flag") == -1);
    REQUIRE(args.get_option("long") == "VALUE");
    REQUIRE(args.get_option("nonexistent-option") == "");
    REQUIRE(!args.get_positionals().empty());
    REQUIRE(args.get_positionals().back() == "positional");
    REQUIRE(ss.str() == "Usage: program [--help|-h] [--long|-l METAVAR] [--verbose|-v]\n");
}


TEST_CASE("ArgumentParser::print_help", "[ArgumentParser]")
{
    const int argc = 1;
    char* argv[] = {(char*)"program"};
    std::stringstream ss;

    ArgumentParser parser(argc, argv);
    parser.add_description("<description>");
    parser.add_example("<example>");
    parser.add_argument("long", 'l', required_argument, "<help 1>", "METAVAR");
    parser.add_argument("verbose", 'v', no_argument, "<help 2>");
    parser.add_epilog("<epilog>");
    Args args = parser.parse_args();

    parser.print_help(ss);

    REQUIRE(ss.str() == "Usage: program [--help|-h] [--long|-l METAVAR] [--verbose|-v]\n\n"
                        "<description>\n\n"
                        "Example: <example>\n\n"
                        "Optional arguments:\n"
                        "  -h, --help         show this help message and exit\n"
                        "  -l, --long=METAVAR <help 1>\n"
                        "  -v, --verbose      <help 2>\n\n"
                        "<epilog>\n");
}
