#ifndef COLORIZE_POSIX_H
#define COLORIZE_POSIX_H

#include <regex.h>
#include <sys/types.h>

#include "Colorize.hpp"


class ColorizePosix : public Colorize
{
public:
    ColorizePosix(const std::string&, const std::string&, std::ostream&);

    void colorize(const std::string&) const;

private:
    regex_t _regex;
};

#endif
