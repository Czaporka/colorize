#include "ArgumentParser.hpp"


ArgumentParser::ArgumentParser(int &argc, char **argv)
{
    for (int i = 1; i < argc; ++i)
        this->tokens.push_back(std::string(argv[i]));
}


std::list<std::string>& ArgumentParser::get_tokens()
{
    return this->tokens;
}


bool ArgumentParser::pop_option(const std::string &option)
{
    auto it = std::find(this->tokens.begin(), this->tokens.end(), option);
    if (it == this->tokens.end())
        return false;
    this->tokens.erase(it);
    return true;
}
