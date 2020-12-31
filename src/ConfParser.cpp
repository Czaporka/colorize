#include <fstream>
#include <sstream>
#include <string>

#include <pwd.h>  // for `passwd` etc.
#include <unistd.h>  // for `getuid`

#include "ConfParser.hpp"


/**
 * Parse the config file located under `path`, and return it as a `map`.
 */
std::map<std::string, std::string> ConfParser::parse_config(std::string path)
{
    std::ifstream in_file(path);
    std::map<std::string, std::string> m;

    if (!in_file.good())
        // if the file doesn't exist, return the empty map
        return m;
    else {
        std::string line;

        while (std::getline(in_file, line)) {

            // skip comments
            if (line.front() == COMMENT_CHARACTER)
                continue;

            std::istringstream iss(line);
            std::string key;
            std::string value;

            std::getline(iss, key, '=');

            if (key == line)
                // didn't find = character at all - skip
                continue;

            std::getline(iss, value);

            m.emplace(key, value);
        }
        return m;
    }
}

/**
 * Parse the config file located under `path`, and return it as a `map`.
 */
std::map<std::string, std::string> ConfParser::parse_config()
{
    return parse_config(_get_conf_file_path());
}

/**
 * Figure out where the config file is.
 */
std::string ConfParser::_get_conf_file_path(void)
{
    const std::string home = _get_home_dir();
    return home + "/" + CONFIG_FILENAME;
}

/**
 * Figure out where the home directory is.
 */
std::string ConfParser::_get_home_dir(void)
{
    const char* home = getenv("HOME");

    if (home != NULL)
        return home;
    else
        return getpwuid(getuid())->pw_dir;
}
