#include <array>
#include <regex>

#define COLORS_SIZE 12

const static std::array<int, COLORS_SIZE> COLORS = {
  //30,  // Black
    31,  // Red
    32,  // Green
    33,  // Yellow
    34,  // Blue
    35,  // Magenta
    36,  // Cyan
  //37,  // White
  //90,  // Bright Black
    91,  // Bright Red
    92,  // Bright Green
    93,  // Bright Yellow
    94,  // Bright Blue
    95,  // Bright Magenta
    96,  // Bright Cyan
  //97,  // Bright White
};

const static std::regex REGEX_NUM("\\b(?:0[xo])?[0-9.a-f]+\\b", std::regex::icase);
const static std::regex REGEX_ALL("\\b[0-9.a-zA-Z]+\\b", std::regex::icase);
