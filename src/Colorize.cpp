#include <iostream>
#include <map>
#include <set>

#include "Colorize.hpp"


int Colorize::_get_color(const std::string& s) const
{
    size_t hash = std::hash<std::string>{}(s + _salt);
    return COLORS[hash % COLORS_SIZE];
}


bool Colorize::_is_worth_colorizing(const std::string& s)
{
    // avoid colorizing dots that are not part of a number
    for (const char& c : s)
        if (isalnum(c))
            return true;
    return false;
}


std::string Colorize::_sanitize(const std::string& chars)
{
    LOG("...");

    // remove duplicate characters by converting to set
    std::set<char> set;
    for (const char& c : chars)
        set.insert(c);

    std::string retval;
    bool hyphen = false;

    for (const char& c : set) {
        switch (c) {
        case '\\':
            retval += "\\\\";
            break;
        case ']':
            retval += "\\";
            retval += c;
            break;
        case '-':
            // we place the hyphen at the end of the character class
            //   to avoid "invalid range" errors
            hyphen = true;
            break;
        default:
            retval += c;
        }
    }
    if (hyphen)
        retval += '-';
    return retval;
}


const std::string Colorize::make_regex(
    const std::string& custom_regex, const std::string& include,
    const bool embedded, const bool all, const bool hex)
{
    LOG("...");

    if (custom_regex != "")
        return custom_regex;
    else {
        // what custom characters do we want to include in the character class?
        const std::string& include2 = _sanitize(include);

        // do we want word boundaries and hexadecimal digits?
        const std::string& boundary = embedded ? "" : "\\b";

        // do we want just numbers or all words?
        if (all)
            return boundary + "[0-9.a-z_" + include2 + "]+" + boundary;
        else {
            if (hex)
                return boundary + "(0[xob])?[0-9.a-f" + include2 + "]+" + boundary;
            else {
                const std::string& hex = boundary + "(0[xob])[0-9.a-f" + include2 + "]+" + boundary;
                const std::string& dec = boundary + "[0-9."              + include2 + "]+" + boundary;
                return hex + "|" + dec;
            }
        }
    }
}
