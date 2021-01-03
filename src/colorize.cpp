#include <iostream>
#include <map>
#include <set>

#include "ArgumentParser.hpp"
#include "colorize.hpp"


int get_next_color()
{
    static std::array<int, COLORS_SIZE>::const_iterator it = COLORS.begin();

    if (it == COLORS.end())
        it = COLORS.begin();
    return *it++;
}


int get_color_cycle(const std::string& s)
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


int get_color_hash(const std::string& s)
{
    int hash = std::hash<std::string>{}(s);
    return COLORS[hash % COLORS_SIZE];
}


void colorize(const std::string& s, const std::regex& regex, int (&get_color)(const std::string&))
{
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


int main(int argc, char **argv)
{
    ArgumentParser parser(argc, argv);
    parser.add_description("Colorize text (by default just numerals).");
    parser.add_example("df | " + std::string(argv[0]));
    parser.add_example("colorize -a < file.txt");
    parser.add_argument("all", 'a', no_argument, "colorize all words, not just numerals");
    parser.add_argument("cycle", 'c', no_argument, "cycle through the list of colors rather than pick the color based on the string's hash; this option makes the token-to-color mapping inconsistent across executions, but is useful when numbers in the input only span a narrow range");
    Args args = parser.parse_args();

    const static std::regex& regex = (args.flags["a"])
        ? REGEX_ALL
        : REGEX_NUM;

    int (&get_color)(const std::string&) = (args.flags["c"])
        ? get_color_cycle
        : get_color_hash;


    while (!std::cin.eof()) {
        std::string line;
        getline(std::cin, line);

        if (std::cin.fail())
            break;
        colorize(line, regex, get_color);
    }
    return 0;
}
