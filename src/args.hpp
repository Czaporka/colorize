#ifndef ARGS_H
#define ARGS_H

#include <getopt.h>
#include <iostream>
#include <unordered_map>
#include <vector>


class Args
{
private:
    friend class Builder;

    // flag long name -> number of occurrences
    std::unordered_map<std::string, int> _flags;

    // this could be a multimap if we wanted to handle situations like `grep -e ... -e ...`
    std::unordered_map<std::string, std::string> _options;
    std::vector<std::string> _positionals;

    Args(const std::vector<option>&);

public:
    class Builder;

    int get_flag(const std::string&);
    std::string get_option(const std::string&);

    const std::unordered_map<std::string, int>& get_flags(void) const { return _flags; };
    const std::unordered_map<std::string, std::string>& get_options(void) const { return _options; };
    const std::vector<std::string>& get_positionals(void) const { return _positionals; }
};


class Args::Builder
{
private:
    Args _args;

public:
    Builder(const std::vector<option>& options) : _args(Args(options)) {}

    void add_flag(const std::string& flag) { ++_args._flags[flag]; }
    void add_option(const std::string& k, const std::string& v) { _args._options.emplace(k, v); }
    void add_positional(const std::string& item) { _args._positionals.push_back(item); }
    const Args build(void) const { return _args; }
};


std::ostream& operator<<(std::ostream&, const Args&);


#endif
