#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <getopt.h>
#include <unordered_map>
#include <string>
#include <vector>

#include "args.hpp"
#include "utils.hpp"


const std::string upper(const std::string& s);


class ArgumentError : public std::runtime_error
{
public:
    ArgumentError(const std::string what)
        : std::runtime_error(what) { }
};


class Arg
{
private:
    const option _option;
    const std::string _metavar;
    const std::string _help;
    const bool _long_only;

public:
    Arg(option& option_, const std::string& help, const std::string& metavar, bool long_only)
        : _option(option_),
          _metavar(metavar),
          _help(help),
          _long_only(long_only) { }

    const std::string get_help(void) const { return _help; }
    const std::string get_metavar(void) const { return _metavar; }
    const option get_option(void) const { return _option; }
    bool is_long_only(void) const { return _long_only; }
};


class ArgumentParser
{
private:
    const int _argc;
    char** _argv;
    int _next_long_only_val = 128;
    const static int _max_help_width = 79;
    std::vector<Arg> _arguments;
    std::string _description;
    std::vector<std::string> _examples;
    std::vector<option> _long_options;
    std::unordered_map<int, std::string> _short_to_long;
    std::string _epilog = "";

    std::string _get_short_options(void);
    const std::string _get_wrapped_help(const std::string&, int);

public:
    ArgumentParser(const int argc, char** argv);
    void print_help(void) { print_help(std::cerr); }
    void print_help(std::ostream&);
    void print_usage(std::ostream&);
    void add_argument(const char* long_, const int argument_required, const std::string& help);
    void add_argument(const char* long_, const int argument_required, const std::string& help, const std::string& metavar);
    void add_argument(const char* long_, const char short_, const int argument_required, const std::string& help);
    void add_argument(const char* long_, const char short_, const int argument_required, const std::string& help, const std::string& metavar);
    void add_description(const std::string& description);
    void add_epilog(const std::string& epilog);
    void add_example(const std::string& example);
    Args parse_args(void);
};


#endif
