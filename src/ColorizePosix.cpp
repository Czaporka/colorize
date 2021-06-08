#include "ColorizePosix.hpp"


void ColorizePosix::colorize(const std::string& s) const
{
    LOG("s=\"" << s << "\"");

    const char* c_str = s.c_str();
    regmatch_t match;

    while (regexec(&_regex, c_str, 1, &match, 0) == 0) {
        LOG("so=" << match.rm_so << ", eo=" << match.rm_eo);

        // output stuff preceding the match
        _os << std::string(c_str, match.rm_so);

        // output the match
        std::string token(c_str + match.rm_so, match.rm_eo - match.rm_so);
        if (_is_worth_colorizing(token))
            _os << "\033[" << _get_color(token) << "m" << token << "\033[0m";
        else
            _os << token;

        c_str += match.rm_eo;

        if (match.rm_so == match.rm_eo)
            c_str++;
    }
    _os << c_str << std::endl;
}


ColorizePosix::ColorizePosix(const std::string& sregex,
                             const std::string& salt, std::ostream& os)
    : Colorize(sregex, salt, os)
{
    LOG("...");
    const int flags = REG_EXTENDED | REG_ICASE;
    const int retval = regcomp(&_regex, sregex.c_str(), flags);
    LOG("retval: " << retval);
    if (retval) {
        LOG("regex: \"" << sregex << "\"");
        std::cerr << "ERROR: Could not compile regex.\n";

        char buf[256];
        regerror(retval, &_regex, buf, sizeof(buf));
        std::cerr << "       " << buf << "\n";

        exit(EXIT_FAILURE);
    }
}
