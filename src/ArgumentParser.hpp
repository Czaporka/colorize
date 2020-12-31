#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <algorithm>
#include <list>
#include <string>


class ArgumentParser
{
private:
    std::list<std::string> tokens;

public:
    ArgumentParser(int &argc, char **argv);
    bool pop_option(const std::string &option);
    std::list<std::string>& get_tokens();
};

#endif
