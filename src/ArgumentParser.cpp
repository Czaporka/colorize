#include <iomanip>
#include <iostream>
#include <sstream>
#include <string.h>

#include "ArgumentParser.hpp"


const std::string upper(const std::string& s)
{
    std::string retval;
    for (char c : s)
        retval += toupper(c);
    return retval;
}


ArgumentParser::ArgumentParser(int argc, char** argv) : _argc(argc), _argv(argv)
{
    option opt = {"help", no_argument, NULL, 'h'};
    _long_options.push_back(opt);
    _arguments.push_back({opt, "show this help message and exit", ""});
}


void ArgumentParser::print_help(void)
{
    print_usage();

    std::cerr << '\n' << _description << "\n\n";

    // print examples
    for (auto& example : _examples)
        std::cerr << "Example: " << example << "\n";

    // get the largest option name length
    int max = 0;
    for (auto& arg: _arguments) {
        int size = strlen(arg.get_option().name);

        if (arg.get_option().has_arg)
            size = (size << 1) + 1;  // for options with args we append "=REPEATED_NAME"

        if (size > max)
            max = size;
    }

    std::cout << "\nOptional arguments:\n";
    for (Arg& arg : _arguments) {
        const int offset = 8;  // the whitespace and the short option and the dashes take up this much width
        std::string help = _get_wrapped_help(arg.get_help(), max + offset);
        std::string long_ = arg.get_option().name + (arg.get_option().has_arg ? ('=' + arg.get_metavar()) : "");

        std::cout
            << "  -" << (char)arg.get_option().val
            << ", --" << std::left << std::setw(max) << long_
            << help << '\n';
    }
}


const std::string ArgumentParser::_get_wrapped_help(const std::string& help, int start)
{
    if (help.size() + 1 <= max_help_width)
        return " " + help;
    else {
        const int max_width = max_help_width - start;

        std::stringstream ss(help);
        std::string s;
        int current_width = 0;

        std::string retval;

        while (std::getline(ss, s, ' ')) {
            int size = s.size() + 1;
            if (current_width + size > max_width) {
                retval += '\n' + std::string(start, ' ');
                current_width = size;
            }
            retval += ' ' + s;
            current_width += size;
        }
        return retval;
    }
}


void ArgumentParser::print_usage(void)
{
    std::cout << "Usage: " << _argv[0];
    for (auto& arg : _arguments) {
        std::cout << " [--" << arg.get_option().name << "|-" << (char)arg.get_option().val;
        if (arg.get_option().has_arg)
            std::cout << ' ' << arg.get_metavar();
        std::cout << ']';
    }
    std::cout << '\n';
}


void ArgumentParser::add_argument(
    const char* long_, const char short_, const int argument_required, const std::string& help)
{
    std::string metavar = argument_required ? upper(long_) : "";
    add_argument(long_, short_, argument_required, help, metavar);
}


void ArgumentParser::add_argument(
    const char* long_, const char short_, const int argument_required, const std::string& help, const std::string& metavar)
{
    for (Arg& arg : _arguments)
        if (arg.get_option().name == long_ || arg.get_option().val == short_)
            throw ArgumentError(std::string("argument --") + arg.get_option().name
                                + ": conflicting with option --" + long_);
    option opt = {long_, argument_required, NULL, short_};
    _long_options.push_back(opt);
    _arguments.push_back({opt, help, metavar});
}


void ArgumentParser::add_description(const std::string& description)
{
    _description = description;
}


void ArgumentParser::add_example(const std::string& example)
{
    _examples.push_back(example);
}


std::string ArgumentParser::_get_short_options(void)
{
    std::string short_options;
    for (const Arg& arg : _arguments) {
        if (arg.get_option().val)
            short_options.push_back(arg.get_option().val);
        if (arg.get_option().has_arg)
            short_options.push_back(':');
    }
    return short_options;
}


Args ArgumentParser::parse_args()
{
    opterr = 0;

    std::string short_options = _get_short_options();

    _long_options.push_back({0, 0, 0, 0});  // push the terminating entry
    const struct option* long_options = &_long_options[0];

    Args args;

    while (true) {
        int option_index = 0;
        int c = getopt_long(_argc, _argv, short_options.c_str(), long_options, &option_index);
        if (c == -1)
            break;
        switch (c) {
        case 'h':
            print_help();
            std::exit(EXIT_SUCCESS);
            break;
        case '?':
            if (short_options.find(optopt) != std::string::npos)
                throw ArgumentError(std::string("option requires an argument -- '") + _argv[optind-1] + "'");
            else
                throw ArgumentError(std::string("unrecognized option: '") + _argv[optind-1] + "'");
            break;
        default:
            if (optarg == NULL)
                ++args.flags[std::string(1, c)];
            else
                args.options.emplace(std::string(1, c), optarg);
            break;
        }
    }
    if (optind < _argc) {
        while (optind < _argc)
            args.positionals.push_back(_argv[optind++]);
    }
    return args;
}
