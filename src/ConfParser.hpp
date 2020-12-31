#ifndef CONFPARSER_H
#define CONFPARSER_H

#include <map>


const static char COMMENT_CHARACTER = '#';
const static std::string CONFIG_FILENAME = ".colorize.conf";


// class Foo
// {
//      X x_;  // private member
// public:
//           X & x()       { return x_; }  // "setter"
//     const X & x() const { return x_; }  // "getter"
// };

// class Config
// {
// private:


// public:

// };

class ConfParser
{
private:
    std::map<std::string, std::string> _m;

    std::string _get_home_dir(void);
    std::string _get_conf_file_path(void);

public:
    std::map<std::string, std::string> parse_config(std::string);  // TODO: return a Config, not map
    std::map<std::string, std::string> parse_config();
};

#endif
