#include "ColorizeCxx.hpp"


void ColorizeCxx::colorize(const std::string& s) const
{
    const static std::sregex_token_iterator end;

    std::sregex_token_iterator it(s.begin(), s.end(), _regex, {-1, 0});
    while (it != end) {
        if (it == end)
            break;
        _os << *it++;
        if (it == end)
            break;
        if (_is_worth_colorizing(*it))
            _os << "\033[" << _get_color(*it) << "m" << *it++ << "\033[0m";
        else
            _os << *it++;
    }
    _os << std::endl;
}


void operator<<(const ColorizeCxx& colorize, std::string& s)
{
    colorize.colorize(s);
}
