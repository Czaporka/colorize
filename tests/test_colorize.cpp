#include "lib/catch2/catch_amalgamated.hpp"

#include "../src/colorize_cxx.hpp"
#include "../src/colorize_posix.hpp"


TEST_CASE("ColorizeCxx::colorize", "[ColorizeCxx]")
{
    std::stringstream stream;
    const std::string& input = "A a 1\n";

    const std::string& s = ColorizeCxx::make_regex();
    ColorizeCxx colorize(s, stream, "", false);
    colorize.colorize(input);

    REQUIRE(stream.str() == "A a \x1b[96m1\x1b[0m\n\n");
}


TEST_CASE("ColorizePosix::colorize", "[ColorizePosix]")
{
    std::stringstream stream;
    const std::string& input = "A a 1\n";

    const std::string& s = ColorizePosix::make_regex();
    ColorizePosix colorize(s, stream, "", false);
    colorize.colorize(input);

    REQUIRE(stream.str() == "A a \x1b[96m1\x1b[0m\n\n");
}


TEST_CASE("ColorizePosix::colorize - true/false/null", "[ColorizePosix]")
{
    std::stringstream stream;
    const std::string& input = "True False FALSE tRuE NULL\n";

    const std::string& s = ColorizePosix::make_regex();
    ColorizePosix colorize(s, stream, "", false);
    colorize.colorize(input);

    REQUIRE(stream.str() ==
        "\x1b[32mTrue\x1b[0m "
        "\x1b[31mFalse\x1b[0m "
        "\x1b[31mFALSE\x1b[0m "
        "\x1b[32mtRuE\x1b[0m "
        "\x1b[90mNULL\x1b[0m\n\n");
}
