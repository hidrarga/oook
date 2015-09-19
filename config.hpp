#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <map>
#include <iostream>
#include <algorithm>

class Config 
{
protected:
    Config() : _map() {}
        
public:
    static Config &instance() {
        static Config instance;
        
        return instance;
    }
    
    void operator=(Config const&) = delete;
    
    static void clear() {
        instance()._map.clear();
    }
    
    static std::string const& get(std::string key) {
        std::transform(key.begin(), key.end(), key.begin(), toupper);
        
        return instance()._map.at(key);
    }
    
    static void set(std::string key, std::string const& value) {
        std::transform(key.begin(), key.end(), key.begin(), toupper);
        
        instance()._map[key] = value;
    }
    
    static bool exists(std::string key) {
        std::transform(key.begin(), key.end(), key.begin(), toupper);
        
        return instance()._map.find(key) != instance()._map.end();
    }
    
    static std::map<std::string, std::string> const& map() {
        return instance()._map;
    }
protected:
    std::map<std::string, std::string> _map;
};

#endif