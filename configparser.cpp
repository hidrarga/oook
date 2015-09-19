#include "configparser.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include "config.hpp"
#include "utils.hpp"

void ConfigParser::read(std::string const &filename)
{
    std::ifstream file(filename.c_str());
    
    if(!file)
        throw std::runtime_error("Error: Can't read config file " + filename);
    
    for(std::string line; std::getline(file, line); ) 
    {
        std::istringstream stream(line);
        
        std::string key;
        std::string value;
        
        stream >> key;
        std::getline(stream, value);
        
        Config::set(key, Utils::trim(value));
    }
    
    file.close();
}

void ConfigParser::write(const std::string& filename)
{
    std::ofstream file(filename.c_str());
    
    if(!file)
        throw std::runtime_error("Error: Can't write config file " + filename);
    
    for(auto const& pair: Config::map())
        file << pair.first << " " << pair.second << std::endl;
    
    file.close();
}