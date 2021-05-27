#ifndef UTILS_H
#define UTILS_H

#ifdef DEBUG
    #define LOG(s) do { std::cerr << "[::" << __FUNCTION__ << "]: " << s << std::endl; } while (false)
#else
    #define LOG(s)
#endif

#endif
