#ifndef COLORIZE_CXX_H
#define COLORIZE_CXX_H

#include <regex>

#include "colorize.hpp"


class ColorizeCxx : public Colorize
{
public:
    ColorizeCxx(const std::string& sregex, std::ostream& os,
                const std::string& salt, bool ignore_case)
        : Colorize(sregex, os, salt, ignore_case),
          _regex(std::regex(sregex, std::regex::icase)) { }

    void colorize(const std::string&) const override;

private:
    const std::regex _regex;
};

#endif
