#include <iomanip>
#include <string.h>
#include <sstream>

#include "argument_parser.hpp"


const std::string upper(const std::string& s)
{
    std::string retval;
    for (char c : s)
        retval += toupper(c);
    return retval;
}


ArgumentParser::ArgumentParser(const int argc, char** argv)
    : _argc(argc),
      _argv(argv)
{
    option opt = {"help", no_argument, NULL, 'h'};
    _long_options.push_back(opt);
    _arguments.push_back({opt, "show this help message and exit", "", false});
    _short_to_long.emplace('h', "help");
}


void ArgumentParser::print_help(std::ostream& os)
{
    print_usage(os);

    os << '\n' << _description << "\n\n";

    // print examples
    for (const auto& example : _examples)
        os << "Example: " << example << "\n";

    // get the largest option name length
    size_t max = 0;
    for (const auto& arg: _arguments) {
        size_t size = strlen(arg.get_option().name);
        if (arg.get_option().has_arg)
            // for options with args we append "=METAVAR"
            size += arg.get_metavar().size() + 1;
        if (size > max)
            max = size;
    }

    os << "\nOptional arguments:\n";
    for (Arg& arg : _arguments) {
        const int offset = 8;  // the whitespace and the short option and the dashes take up this much width
        std::string help = _get_wrapped_help(arg.get_help(), max + offset);
        std::string long_ = arg.get_option().name + (arg.get_option().has_arg ? ('=' + arg.get_metavar()) : "");

        if (arg.is_long_only())
            os << "     ";
        else
            os << "  -" << (char)arg.get_option().val << ",";
        os << " --" << std::left << std::setw(max) << long_ << help << '\n';
    }

    if (_epilog != "")
        os << '\n' << _epilog << '\n';
}


const std::string ArgumentParser::_get_wrapped_help(const std::string& help, int start)
{
    const int max_width = _max_help_width - start;

    if (static_cast<int>(help.size()) + 1 <= max_width)
        return " " + help;
    else {
        std::stringstream ss(help);
        std::string s;
        int current_width = 0;
        std::string retval;

        while (std::getline(ss, s, ' ')) {
            int size = s.size() + 1;
            if (current_width + size > max_width) {
                retval += '\n' + std::string(start, ' ');
                current_width = 0;
            }
            retval += ' ' + s;
            current_width += size;
        }
        return retval;
    }
}


void ArgumentParser::print_usage(std::ostream& os)
{
    os << "Usage: " << _argv[0];
    for (const auto& arg : _arguments) {
        os << " [--" << arg.get_option().name;
        if (!arg.is_long_only())
            os << "|-" << (char)arg.get_option().val;
        if (arg.get_option().has_arg)
            os << ' ' << arg.get_metavar();
        os << ']';
    }
    os << '\n';
}


/**
 * Add an argument that accepts only a long-style option.
 *
 * Set metavar to long option name converted to uppercase.
 */
void ArgumentParser::add_argument(
    const char* long_, const int argument_required, const std::string& help)
{
    std::string metavar = argument_required ? upper(long_) : "";
    add_argument(long_, argument_required, help, metavar);
}


/**
 * Add an argument that accepts only a long-style option.
 */
void ArgumentParser::add_argument(
    const char* long_, const int argument_required,
    const std::string& help, const std::string& metavar)
{
    for (Arg& arg : _arguments)
        if (arg.get_option().name == long_)
            throw ArgumentError(std::string("argument --") + arg.get_option().name
                                + ": conflicting with option --" + long_);
    option opt = {long_, argument_required, NULL, _next_long_only_val};
    LOG("opt = {" << long_ << ", " << argument_required << ", NULL, " << _next_long_only_val << "}");
    _long_options.push_back(opt);
    _arguments.push_back({opt, help, metavar, true});
    _short_to_long.emplace(_next_long_only_val++, long_);
}


/**
 * Add an argument that accepts both a short- and a long-style option.
 *
 * Set metavar to long option name converted to uppercase.
 */
void ArgumentParser::add_argument(
    const char* long_, const char short_, const int argument_required,
    const std::string& help)
{
    std::string metavar = argument_required ? upper(long_) : "";
    add_argument(long_, short_, argument_required, help, metavar);
}


/**
 * Add an argument that accepts both a short- and a long-style option.
 */
void ArgumentParser::add_argument(
    const char* long_, const char short_, const int argument_required,
    const std::string& help, const std::string& metavar)
{
    for (Arg& arg : _arguments)
        if (arg.get_option().name == long_ || arg.get_option().val == short_)
            throw ArgumentError(std::string("argument --") + arg.get_option().name
                                + ": conflicting with option --" + long_);
    option opt = {long_, argument_required, NULL, short_};
    LOG("opt = {" << long_ << ", " << argument_required << ", NULL, " << short_ << "}");
    _long_options.push_back(opt);
    _arguments.push_back({opt, help, metavar, false});
    _short_to_long.emplace(short_, long_);
}


void ArgumentParser::add_description(const std::string& description)
{
    _description = description;
}


/**
 * Add an example to be displayed as part of the help message.
 */
void ArgumentParser::add_example(const std::string& example)
{
    _examples.push_back(example);
}


void ArgumentParser::add_epilog(const std::string& epilog)
{
    _epilog = epilog;
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
    LOG("...");

    opterr = 0;

    std::string short_options = _get_short_options();

    _long_options.push_back({0, 0, 0, 0});  // push the terminating entry
    const struct option* long_options = &_long_options[0];

    Args::Builder builder(_long_options);

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
        case '?': {
            std::string msg = (short_options.find(optopt) != std::string::npos)
                ? std::string("option requires an argument -- '") + _argv[optind-1] + "'"
                : std::string("unrecognized option: '") + _argv[optind-1] + "'";
            // throw ArgumentError(msg);
            std::cerr << _argv[0] << ": error: " << msg << '\n';
            print_usage(std::cerr);
            std::cerr << "Try '" << _argv[0] << " --help' for more information.\n";
            std::exit(EXIT_FAILURE);
            break;
        }
        default:
            if (optarg == NULL) {
                LOG("adding flag (optarg=NULL)");
                builder.add_flag(_short_to_long[c]);
            } else {
                LOG("adding option (optarg=" << optarg << ")");
                builder.add_option(_short_to_long[c], optarg);
            }
            break;
        }
    }
    if (optind < _argc) {
        while (optind < _argc) {
            LOG("adding positional (" << _argv[optind] << ")");
            builder.add_positional(_argv[optind++]);
        }
    }

    // "Reset" the state so that later we can try to do another parsing.
    // This may not work on all platforms, but outside of testing scenarios,
    //   it typically doesn't make sense to parse arguments more than once.
    optind = 1;

    return builder.build();
}
