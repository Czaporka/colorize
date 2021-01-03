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


int get_color_cycle(const std::string& s, const std::string& salt)
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


int get_color_hash(const std::string& s, const std::string& salt)
{
    size_t hash = std::hash<std::string>{}(s + salt);
    return COLORS[hash % COLORS_SIZE];
}


void colorize(const std::string& s, const std::regex& regex, int (&get_color)(const std::string&, const std::string&), std::string& salt)
{
    const static std::sregex_token_iterator end;

    std::sregex_token_iterator it(s.begin(), s.end(), regex, {-1, 0});
    while (it != end) {
        if (it == end)
            break;
        std::cout << *it++;
        if (it == end)
            break;
        std::cout << "\033[" << get_color(*it, salt) << "m" << *it++ << "\033[0m";
    }
    std::cout << std::endl;
}


int main(int argc, char** argv)
{
    ArgumentParser parser(argc, argv);
    parser.add_description("Colorize text (by default just numerals).");
    parser.add_example("df | " + std::string(argv[0]));
    parser.add_example(std::string(argv[0]) + " -a < file.txt");
    parser.add_argument("all", 'a', no_argument, "colorize all words, not just numerals");
    parser.add_argument("cycle", 'c', no_argument,
                        "cycle through the list of colors rather than pick the color based on the token's hash; "
                        "this option makes token-to-color mapping inconsistent across executions and is probably not useful at all");
    parser.add_argument("regex", 'r', required_argument,
                        "use an arbitrary REGEX for finding the tokens");
    parser.add_argument("salt", 's', required_argument,
                        "append a salt to every token before hashing, effectively shuffling the colors");
    Args args = parser.parse_args();

    std::string custom_regex = args.options['r'];

    const static std::regex& regex = (custom_regex != "")
        ? std::regex(custom_regex)
        : (args.flags['a'])
            ? REGEX_ALL
            : REGEX_NUM;

    int (&get_color)(const std::string&, const std::string&) = (args.flags['c'])
        ? get_color_cycle
        : get_color_hash;

    std::string salt = args.options['s'];

    while (!std::cin.eof()) {
        std::string line;
        getline(std::cin, line);

        if (std::cin.fail())
            break;
        colorize(line, regex, get_color, salt);
    }
    return 0;
}
