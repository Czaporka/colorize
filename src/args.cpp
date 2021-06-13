#include <iostream>

#include "args.hpp"


Args::Args(const std::vector<option>& options) {
    // initialize all flags to 0
    for (auto it = options.begin(); it != options.end() - 1; ++it) {
        _flags[it->name] = 0;
    }
}


int Args::get_flag(const std::string& flag)
{
    auto tmp = _flags.find(flag);
    if (tmp == _flags.end())
        return -1;
    return tmp->second;
}


std::string Args::get_option(const std::string& option)
{
    auto tmp = _options.find(option);
    if (tmp == _options.end())
        return "";
    return tmp->second;
}


std::ostream& operator<<(std::ostream& os, const Args& args)
{
    const auto& flags = args.get_flags();
    const auto& options = args.get_options();
    const auto& positionals = args.get_positionals();

    os << "Args(flags={";
    for (auto it = flags.begin(); it != flags.end(); ++it) {
        os << it->first << '=' << it-> second;
        if (std::next(it) != flags.end())
            os << ", ";
    }
    os << "}, options={";
    for (auto it = options.begin(); it != options.end(); ++it) {
        os << it->first << "=\"" << it-> second << "\"";
        if (std::next(it) != options.end())
            os << ", ";
    }
    os << "}, positionals=[";
    for (auto it = positionals.begin(); it != positionals.end(); ++it) {
        os << '"' << *it << '"';
        if (std::next(it) != positionals.end())
            os << ", ";
    }
    os << "])";
    return os;
}
