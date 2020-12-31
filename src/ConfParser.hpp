// #include <algorithm>
#include <fstream>
#include <map>
// #include <list>
// #include <string>


class Foo
{
     X x_;  // private member
public:
          X & x()       { return x_; }  // "setter"
    const X & x() const { return x_; }  // "getter"
};

class Config
{
private:


public:

};

class ConfParser
{
private:
    std::map<std::string, std::string> m;

public:
    ArgumentParser(int &argc, char **argv);
    bool pop_option(const std::string &option);
    std::list<std::string>& get_tokens();
};
