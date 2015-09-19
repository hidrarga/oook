#ifndef __CONFIGPARSER_H__
#define __CONFIGPARSER_H__

#include <string>

class ConfigParser {
public:
    void read(std::string const& filename);
    void write(std::string const& filename);
};

#endif