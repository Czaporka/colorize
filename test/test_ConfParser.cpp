#define CATCH_CONFIG_MAIN
#include "lib/catch2/catch_amalgamated.hpp"

#include "../src/ConfParser.hpp"


TEST_CASE("ConfigParser::parse_config(std::string)", "[ConfigParser]" ) {
    auto path = "test/data/.colorize.conf";

    auto parser = ConfParser();
    auto config = parser.parse_config(path);

    REQUIRE(!config.empty());
    REQUIRE(config["a"] == "123");
    REQUIRE(config["b"] == " 234");
    REQUIRE(config["c"] == "345");
    REQUIRE(config["d"] == "");
    REQUIRE(config["e "] == " 7");
}
