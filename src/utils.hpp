#ifndef UTILS_H
#define UTILS_H

#ifdef DEBUG
    #define LOG(s) do { std::cerr << "\033[33m" << __FILE__ << ':' << __LINE__ << "(" << __FUNCTION__ << "): " << s << "\033[0m\n"; } while (false)
#else
    #define LOG(s)
#endif

#endif
