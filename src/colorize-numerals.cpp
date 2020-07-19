#include <iostream>
#include <map>
#include <regex>
#include <set>

#include "colorize-numerals.hpp"


int get_next_color()
{
    static std::array<int, COLORS_SIZE>::const_iterator it = COLORS.begin();

    if (it == COLORS.end())
        it = COLORS.begin();
    return *it++;
}


int get_color(std::string s)
{
    static std::map<std::string, int> m;  // token-to-color mapping

    auto it = m.find(s);
    if (it != m.end())
        return it->second;
    else {
        int color = get_next_color();
        m.emplace(s, color);
        return color;
    }
}


void colorize(std::string s)
{
    const static std::regex regex("\\b(?:0[xo])?[0-9.a-f]+\\b", std::regex::icase);
    const static std::sregex_token_iterator end;

    std::sregex_token_iterator it(s.begin(), s.end(), regex, {-1, 0});
    while (it != end) {
        if (it == end)
            break;
        std::cout << *it++;
        if (it == end)
            break;
        std::cout << "\033[" << get_color(*it) << "m" << *it++ << "\033[0m";
    }
    std::cout << std::endl;
}


int main()
{
    while (!std::cin.eof()) {
        std::string line;
        getline(std::cin, line);

        if (std::cin.fail())
            break;
        colorize(line);
    }
    return 0;
}
