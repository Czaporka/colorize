#include <algorithm>
#include <iostream>
#include <map>
#include <set>

#include "Colorize.hpp"


int Colorize::_get_color(const std::string& s) const
{
    std::string lower;
    size_t hash;

    // handle special tokens such as True, False, Null
    lower = std::string(s);
    std::transform(s.begin(), s.end(), lower.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    hash = std::hash<std::string>{}(lower + _salt);
    auto tmp = _special_tokens.find(hash);
    if (tmp != _special_tokens.end())
        return tmp->second;

    if (!_ignore_case)
        hash = std::hash<std::string>{}(s + _salt);

    return COLORS[hash % COLORS_SIZE];
}


const std::unordered_map<size_t, int> Colorize::_get_special_tokens(std::string salt)
{
    std::unordered_map<size_t, int> m;
    for (auto &p : SPECIAL_TOKENS)
        m.emplace(std::hash<std::string>{}(p.first + salt), p.second);
    return m;
}


bool Colorize::_is_worth_colorizing(const std::string& s)
{
    // avoid colorizing punctuation characters that are not part of a word
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
    const std::string& custom_regex, const std::string& comprise,
    const bool embedded, const bool all, const bool hex)
{
    LOG("...");

    if (custom_regex != "")
        return custom_regex;
    else {
        // what custom characters do we want to include in the character class?
        const std::string& comprise2 = _sanitize(comprise);

        // do we want word boundaries and hexadecimal digits?
        const std::string& boundary = embedded ? "" : "\\b";

        // do we want just numbers or all words?
        if (all)
            return boundary + "[0-9.a-z_" + comprise2 + "]+" + boundary;
        else {
            if (hex)
                return boundary + "(0[xob])?[0-9.a-f" + comprise2 + "]+" + boundary;
            else {
                const std::string& hex = boundary + "(0[xob])[0-9.a-f" + comprise2 + "]+" + boundary;
                const std::string& dec = boundary + "[0-9."            + comprise2 + "]+" + boundary;
                std::string s = hex + '|' + dec;
                for (auto &p : SPECIAL_TOKENS)
                    s += '|' + p.first;
                return s;
            }
        }
    }
}
