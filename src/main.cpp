#include "ArgumentParser.hpp"
#include "Colorize.hpp"

#if !defined(USE_REGEX_H) && !defined(USE_GLIBCXX_REGEX)
#  error: USE_REGEX_H or USE_GLIBCXX_REGEX has to be defined.
#endif

#ifdef USE_REGEX_H
#  include "ColorizePosix.hpp"
#endif

#ifdef USE_GLIBCXX_REGEX
#  include "ColorizeCxx.hpp"
#endif


int main(int argc, char** argv)
{
    ArgumentParser parser(argc, argv);
    parser.add_description("Colorize text (by default just numerals).");
    parser.add_example("df | " + std::string(argv[0]));
    parser.add_example(std::string(argv[0]) + " -a < file.txt");
    parser.add_argument("all", 'a', no_argument,
                        "colorize all words, not just numerals");
    parser.add_argument("comprise", 'c', required_argument,
                        "specify additional characters to be interpreted as"
                        " part of the same token; for example, use --comprise=,:"
                        " for text containing numbers with comma for decimal"
                        " separator (e.g. 3,141) and timestamps such as"
                        " 23:59:59", "CHARS");
    parser.add_argument("embedded", 'e', no_argument,
                        "also catch numbers embedded within larger, not purely"
                        " numerical strings, e.g. in \"Word01\", \"01\" will be"
                        " colorized (normally it would not)");
    parser.add_argument("ignore-case", 'i', no_argument, "make sure \"token\","
                        " \"TOKEN\", \"ToKeN\", etc., are the same color");
    parser.add_argument("print-regex", 'p', no_argument,
                        "print the final regex to STDERR; this can be used to"
                        " fine-tune the regex manually, then provide the result"
                        " with the --regex option");
    parser.add_argument("regex", 'r', required_argument,
                        "use an arbitrary REGEX for finding the tokens; -a and"
                        " -c are then ignored");
    parser.add_argument("salt", 's', required_argument,
                        "append a salt to every token before hashing,"
                        " effectively shuffling the colors");
    parser.add_argument("version", 'V', no_argument,
                        "print version and exit");
    parser.add_argument("hex", 'x', no_argument,
                        "force catching hexadecimal digits even when not"
                        " prefixed with \"0x\"");
    #if defined USE_REGEX_H && defined USE_GLIBCXX_REGEX
    parser.add_argument("posix", no_argument, "use the POSIX regex library");
    #endif
    parser.add_epilog(
        "Homepage: https://github.com/Czaporka/colorize\n"
        "Version: " VERSION "\n"
    );
    Args args = parser.parse_args();
    LOG("args: " << args);

    if (args.flags["version"]) {
        std::cout << "colorize version " VERSION << "\n";
        std::exit(EXIT_SUCCESS);
    }

    Colorize* colorize = nullptr;

    const std::string& regex = colorize->make_regex(
        args.get_option("regex"),
        args.get_option("comprise"),
        args.get_flag("embedded"),
        args.get_flag("all"),
        args.get_flag("hex")
    );
    if (args.flags["print-regex"])
        std::cerr << regex << "\n";

    #if defined(USE_REGEX_H) && defined(USE_GLIBCXX_REGEX)
    if (args.get_flag("posix"))
        colorize = new ColorizePosix(
            regex, std::cout, args.get_option("salt"), args.get_flag("ignore-case"));
    else
        colorize = new ColorizeCxx(
            regex, std::cout, args.get_option("salt"), args.get_flag("ignore-case"));
    #elif defined(USE_REGEX_H)
    colorize = new ColorizePosix(
        regex, std::cout, args.get_option("salt"), args.get_flag("ignore-case"));
    #elif defined(USE_GLIBCXX_REGEX)
    colorize = new ColorizeCxx(
        regex, std::cout, args.get_option("salt"), args.get_flag("ignore-case"));
    #endif

    while (!std::cin.eof()) {
        std::string line;
        getline(std::cin, line);
        if (std::cin.fail())
            break;
        colorize->colorize(line);
    }
    return 0;
}
