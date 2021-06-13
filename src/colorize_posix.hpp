#ifndef COLORIZE_POSIX_H
#define COLORIZE_POSIX_H

#include <regex.h>
#include <sys/types.h>

#include "colorize.hpp"


class ColorizePosix : public Colorize
{
public:
    ColorizePosix(const std::string&, std::ostream&, const std::string&, bool);

    void colorize(const std::string&) const override;

private:
    regex_t _regex;
};

#endif
