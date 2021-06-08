#ifndef COLORIZE_CXX_H
#define COLORIZE_CXX_H

#include <regex>

#include "Colorize.hpp"


class ColorizeCxx : public Colorize
{
public:
    ColorizeCxx(const std::string& sregex, const std::string& salt, std::ostream& os)
        : Colorize(sregex, salt, os),
          _regex(std::regex(sregex, std::regex::icase)) { }

    void colorize(const std::string&) const override;

private:
    const std::regex _regex;
};

#endif
