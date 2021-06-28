#ifndef COLORIZE_H
#define COLORIZE_H

#include "utils.hpp"

#include <array>
#include <iostream>
#include <unordered_map>
#include <set>

#define C_BLACK          30
#define C_RED            31
#define C_GREEN          32
#define C_YELLOW         33
#define C_BLUE           34
#define C_MAGENTA        35
#define C_CYAN           36
#define C_WHITE          37
#define C_BRIGHT_BLACK   90
#define C_BRIGHT_RED     91
#define C_BRIGHT_GREEN   92
#define C_BRIGHT_YELLOW  93
#define C_BRIGHT_BLUE    94
#define C_BRIGHT_MAGENTA 95
#define C_BRIGHT_CYAN    96
#define C_BRIGHT_WHITE   97

#define COLOR_TRUE    C_GREEN
#define COLOR_FALSE   C_RED
#define COLOR_NEUTRAL C_BRIGHT_BLACK

#define COLORS_SIZE 8

const static std::array<int, COLORS_SIZE> COLORS = {
    // C_BLACK,
    //C_RED,
    //C_GREEN,
    C_YELLOW,
    C_BLUE,
    C_MAGENTA,
    C_CYAN,
    // C_WHITE,
    // C_BRIGHT_BLACK,
    //C_BRIGHT_RED,
    //C_BRIGHT_GREEN,
    C_BRIGHT_YELLOW,
    C_BRIGHT_BLUE,
    C_BRIGHT_MAGENTA,
    C_BRIGHT_CYAN,
    // C_BRIGHT_WHITE,
};

// Note: the keys are used in a case-insensitive way
const static std::unordered_map<std::string, int> SPECIAL_TOKENS = {
    // ---- truthy -----
    {"true", COLOR_TRUE},
    {"yes", COLOR_TRUE},
    // ---- falsy ------
    {"false", COLOR_FALSE},
    {"no", COLOR_FALSE},
    // ---- neutral ----
    {"null", COLOR_NEUTRAL},
    {"none", COLOR_NEUTRAL},
    {"0", COLOR_NEUTRAL},
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
    std::ostream& _os;
    const std::string& _salt;
    const bool _ignore_case;
    const std::unordered_map<size_t, int> _special_tokens;

    Colorize(const std::string& sregex, std::ostream& os,
             const std::string& salt, bool ignore_case)
        : _sregex(sregex),
          _os(os),
          _salt(salt),
          _ignore_case(ignore_case),
          _special_tokens(_get_special_tokens(salt)) { }

    int _get_color(const std::string&) const;
    static bool _is_worth_colorizing(const std::string&);
    static std::string _sanitize(const std::string&);

private:
    const std::unordered_map<size_t, int> _get_special_tokens(std::string);
};


#endif
