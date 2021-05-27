#ifndef COLORIZE_H
#define COLORIZE_H

#include "utils.hpp"

#include <array>
#include <iostream>
#include <map>
#include <set>

#define COLORS_SIZE 12

const static std::array<int, COLORS_SIZE> COLORS = {
  //30,  // Black
    31,  // Red
    32,  // Green
    33,  // Yellow
    34,  // Blue
    35,  // Magenta
    36,  // Cyan
  //37,  // White
  //90,  // Bright Black
    91,  // Bright Red
    92,  // Bright Green
    93,  // Bright Yellow
    94,  // Bright Blue
    95,  // Bright Magenta
    96,  // Bright Cyan
  //97,  // Bright White
};


class Colorize
{
public:
    const static std::string make_regex(const std::string&, const std::string&,
                                        const bool, const bool, const bool);
    const static std::string make_regex() { return make_regex("", "", false, false, false); }

    virtual void colorize(const std::string&) const = 0;

protected:
    const std::string& _sregex;
    const std::string& _salt;
    std::ostream& _os;

    Colorize(const std::string& sregex, const std::string& salt, std::ostream& os)
        : _sregex(sregex),
          _salt(salt),
          _os(os) { }

    int _get_color(const std::string&) const;
    static bool _is_worth_colorizing(const std::string&);
    static std::string _sanitize(const std::string&);
};


#endif
