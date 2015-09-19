#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <vector>
#include <map>

class Utils {
public:
    template<template<typename...> class Container>
    static std::string join(const std::string& delimiter, const Container<std::string>& list)
    {
        std::string str;
        
        for(auto it = list.begin(); it != list.end(); ++it) {
            if(it != list.begin())
                str += delimiter;
            str += (*it);
        }
        
        return str;
    }
    
    template<typename T, typename C, template<typename, typename...> class Container>
    static std::string join(const std::string& delimiter, const Container<T*, C>& list)
    {
        std::string str;
        
        for(auto it = list.begin(); it != list.end(); ++it) {
            if(it != list.begin())
                str += delimiter;
            str += (*it)->name();
        }
        
        return str;
    }
    
    static std::vector<std::string> split(char delimiter, const std::string& str);
    
    static bool replace(std::string &str, std::string const& from, std::string const& to);
    static bool replace(std::string &str, std::string const& from, int to) {
        return replace(str, from, std::to_string(to));
    }
    
    static void replaceAll(std::string &str, std::string const& from, std::string const& to);
    static void replaceAll(std::string &str, std::string const& from, int to) {
        replaceAll(str, from, std::to_string(to));
    }
    
    static std::string trim(std::string const& str);
    static std::string plural(std::string const& str, int n = 2);
    static std::string ordinal(int num);
    static std::string titlize(std::string title);
    
    static int hex(char c);
    static std::string decodeURI(std::string const& uri);
    static std::map<std::string, std::string> paramsURI(std::string const& uri);
    static std::string const& append(std::string& str, char c = '/');
};

#endif