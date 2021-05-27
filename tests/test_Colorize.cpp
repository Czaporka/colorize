#define CATCH_CONFIG_MAIN
#include "lib/catch2/catch_amalgamated.hpp"

#include "../src/ColorizeCxx.hpp"
#include "../src/ColorizePosix.hpp"


TEST_CASE("ColorizeCxx::colorize()", "[ColorizeCxx]")
{
    std::stringstream stream;
    const std::string& input = "A a 1\n";

    const std::string& s = ColorizeCxx::make_regex();
    ColorizeCxx colorize(s, "", stream);
    colorize.colorize(input);

    REQUIRE(stream.str() == "A a \x1b[96m1\x1b[0m\n\n");
}


TEST_CASE("ColorizePosix::colorize()", "[ColorizePosix]")
{
    std::stringstream stream;
    const std::string& input = "A a 1\n";

    const std::string& s = ColorizePosix::make_regex();
    ColorizePosix colorize(s, "", stream);
    colorize.colorize(input);

    REQUIRE(stream.str() == "A a \x1b[96m1\x1b[0m\n\n");
}
