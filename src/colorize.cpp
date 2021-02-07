#include <iostream>
#include <map>
#include <set>

#include "ArgumentParser.hpp"
#include "colorize.hpp"


int get_color(const std::string& s,
              const std::string& salt)
{
    size_t hash = std::hash<std::string>{}(s + salt);
    return COLORS[hash % COLORS_SIZE];
}


bool is_worth_colorizing(const std::string& s)
{
    // avoid colorizing dots that are not part of a number
    for (const char& c : s)
        if (isalnum(c))
            return true;
    return false;
}


void colorize(const std::string& s,
              const std::regex& regex,
              const std::string& salt)
{
    const static std::sregex_token_iterator end;

    std::sregex_token_iterator it(s.begin(), s.end(), regex, {-1, 0});
    while (it != end) {
        if (it == end)
            break;
        std::cout << *it++;
        if (it == end)
            break;
        if (is_worth_colorizing(*it))
            std::cout << "\033[" << get_color(*it, salt) << "m" << *it++ << "\033[0m";
        else
            std::cout << *it++;
    }
    std::cout << std::endl;
}


std::string sanitize(const std::string& chars)
{
    // remove duplicate characters by converting to set
    std::set<char> set;
    for (const char& c : chars)
        set.insert(c);

    std::string retval;
    bool hyphen = false;

    for (const char& c : set) {
        switch (c) {
        case '\\':
            retval += "\\\\";
            break;
        case ']':
            retval += "\\";
            retval += c;
            break;
        case '-':
            // we place the hyphen at the end of the character class
            //   to avoid "invalid range" errors
            hyphen = true;
            break;
        default:
            retval += c;
        }
    }
    if (hyphen)
        retval += '-';
    return retval;
}


const std::string make_regex(const std::string& custom_regex,
                             const std::string& include,
                             const bool embedded,
                             const bool all,
                             const bool hex)
{
    if (custom_regex != "")
        return custom_regex;
    else {
        // what custom characters do we want to include in the character class?
        const std::string& include2 = sanitize(include);

        // do we want word boundaries and hexadecimal digits?
        const std::string& boundary = embedded ? "" : "\\b";

        // do we want just numbers or all words?
        if (all)
            return boundary + "[0-9.a-z_" + include2 + "]+" + boundary;
        else {
            if (hex)
                return boundary + "(?:0[xob])?[0-9.a-f" + include2 + "]+" + boundary;
            else {
                const std::string& hex = boundary + "(?:0[xob])[0-9.a-f" + include2 + "]+" + boundary;
                const std::string& dec = boundary + "[0-9."             + include2 + "]+" + boundary;
                return hex + "|" + dec;
            }
        }
    }
}


int main(int argc, char** argv)
{
    ArgumentParser parser(argc, argv);
    parser.add_description("Colorize text (by default just numerals).");
    parser.add_example("df | " + std::string(argv[0]));
    parser.add_example(std::string(argv[0]) + " -a < file.txt");
    parser.add_argument("all", 'a', no_argument,
                        "colorize all words, not just numerals");
    parser.add_argument("embedded", 'e', no_argument,
                        "also catch numbers embedded within larger, not purely"
                        " numerical strings, e.g. in \"Word01\", \"01\" will be"
                        " colorized (normally it wouldn't)");
    parser.add_argument("include", 'i', required_argument,
                        "specify additional characters to be interpreted as"
                        " part of the same token; for example, use --include=,:"
                        " for text containing numbers with comma for decimal"
                        " separator (e.g. 3,141) and timestamps such as"
                        " 23:59:59");
    parser.add_argument("print-regex", 'p', no_argument,
                        "print the final regex to STDERR; this can be used to"
                        " fine-tune the regex manually, then provide the result"
                        " with the --regex option");
    parser.add_argument("regex", 'r', required_argument,
                        "use an arbitrary REGEX for finding the tokens; -a and"
                        " -i are ignored");
    parser.add_argument("salt", 's', required_argument,
                        "append a salt to every token before hashing,"
                        " effectively shuffling the colors");
    parser.add_argument("hex", 'x', no_argument,
                        "force catching hexadecimal digits even when not"
                        " prefixed with \"0x\"");
    Args args = parser.parse_args();

    const std::string& s = make_regex(
        args.options.find('r')->second,
        args.options.find('i')->second,
        args.flags.find('e')->second,
        args.flags.find('a')->second,
        args.flags.find('x')->second
    );
    if (args.flags['p'])
        std::cerr << s << "\n";
    const std::regex& regex = std::regex(s, std::regex::icase);
    const std::string& salt = args.options['s'];

    while (!std::cin.eof()) {
        std::string line;
        getline(std::cin, line);
        if (std::cin.fail())
            break;
        colorize(line, regex, salt);
    }
    return 0;
}
