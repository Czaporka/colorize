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


int get_color(const std::string& s)
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


void colorize(const std::string& s, const std::regex& regex)
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


void print_help()
{
    std::cout
        << "Usage: colorize [-a | -h]\n"
        << "Colorize text (by default just numerals).\n"
        << "Example: df | colorize\n"
        << "Example: colorize -a < file.txt\n"
        << "Options:\n"
        << "  -a, --all   : colorize all words, not just numerals\n"
        << "  -h, --help  : display this help message and exit\n";
}


void print_usage()
{
    std::cout
        << "Usage: colorize [-a | -h]\n"
        << "Try 'colorize --help' for more information.\n";
}


int main(int argc, char **argv)
{
    ArgumentParser parser(argc, argv);

    if (parser.pop_option("-h") || parser.pop_option("--help")) {
        print_help();
        return 0;
    }

    const static std::regex& regex = (
        parser.pop_option("-a") || parser.pop_option("--all")) ? REGEX_ALL : REGEX_NUM;

    if (!parser.get_tokens().empty()) {
        print_usage();
        return -1;
    }

    while (!std::cin.eof()) {
        std::string line;
        getline(std::cin, line);

        if (std::cin.fail())
            break;
        colorize(line, regex);
    }
    return 0;
}
