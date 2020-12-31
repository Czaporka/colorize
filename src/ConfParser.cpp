#include <sstream>
#include <string>

#include "ConfParser.hpp"

Config ConfParser::parse_config(void)
{
    std::ifstream infile("f.txt");

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        int a, b;
        if (!(iss >> a >> b)) { break; } // error

        // process pair (a,b)
    }
}

std::string ConfParser::get_conf_file_path(void)
{

}









#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

struct passwd *pw = getpwuid(getuid());

const char *homedir = pw->pw_dir;






#include <stdlib.h>
const char* home = getenv("HOME")
if (home) { /* replace ~/ with home */ }







#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

const char *homedir;

if ((homedir = getenv("HOME")) == NULL) {
    homedir = getpwuid(getuid())->pw_dir;
}