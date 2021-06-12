#ifndef ARGS_H
#define ARGS_H

#include <getopt.h>
#include <iostream>
#include <unordered_map>
#include <vector>


struct Args
{
    // flag long name -> number of occurrences
    std::unordered_map<std::string, int> flags;

    // this could be a multimap if we wanted to handle situations like `grep -e ... -e ...`
    std::unordered_map<std::string, std::string> options;
    std::vector<std::string> positionals;

    Args(const std::vector<option>&);

    int get_flag(const std::string&);
    std::string get_option(const std::string&);
};


std::ostream& operator<<(std::ostream&, const Args&);


#endif
