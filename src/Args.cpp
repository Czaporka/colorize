#include <iostream>

#include "Args.hpp"
#include "utils.hpp"


Args::Args(const std::vector<option>& options) {
    LOG("...");
    // initialize all flags to 0
    for (auto it = options.begin(); it != options.end() - 1; ++it) {
        flags[it->name] = 0;
    }
}


int Args::get_flag(const std::string& flag)
{
    return flags.find(flag)->second;
}


std::string Args::get_option(const std::string& option)
{
    auto tmp = options.find(option);
    if (tmp == options.end())
        return "";
    return tmp->second;
}


std::ostream& operator<<(std::ostream& os, const Args& args)
{
    os << "Args(flags={";
    for (auto it = args.flags.begin(); it != args.flags.end(); ++it) {
        os << it->first << '=' << it-> second;
        if (std::next(it) != args.flags.end())
            os << ", ";
    }
    os << "}, options={";
    for (auto it = args.options.begin(); it != args.options.end(); ++it) {
        os << it->first << "=\"" << it-> second << "\"";
        if (std::next(it) != args.options.end())
            os << ", ";
    }
    os << "}, positionals=[";
    for (auto it = args.positionals.begin(); it != args.positionals.end(); ++it) {
        os << '"' << *it << '"';
        if (std::next(it) != args.positionals.end())
            os << ", ";
    }
    os << "])";
    return os;
}
