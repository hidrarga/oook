#include "utils.hpp"

#include <sstream>
#include <fstream>
#include <iostream>
#include <cstring>

std::vector<std::string> Utils::split(char delimiter, const std::string &str)
{
    std::vector<std::string> list;
    
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delimiter))
        list.push_back(item);
    
    return list;
}

bool Utils::replace(std::string &str, std::string const& from, std::string const& to) {
    std::size_t start_pos = str.find(from);
    
    if(start_pos == std::string::npos)
        return false;
    
    str.replace(start_pos, from.length(), to);
    
    return true;
}

void Utils::replaceAll(std::string &str, std::string const& from, std::string const& to) {
    std::size_t start = 0;
    while(start != std::string::npos) {
        start = str.find(from, start);
        
        if(start != std::string::npos) {
            str.replace(start, from.length(), to);
            start += to.length();
        }
    }
}

std::string Utils::trim(std::string const& str) {
    if(str.empty())
        return str;
    
    std::size_t first = str.find_first_not_of("\t ");
    std::size_t last = str.find_last_not_of("\t ");
    
    return str.substr(first, last-first+1);
}

std::string Utils::plural(const std::string& str, int n)
{
    return (n > 1 and str[str.size()-1] != 's') ? str + 's' : str;
}

std::string Utils::ordinal(int num) {
    std::string index = std::to_string(num);
        
    if(index.back() == '1')
        index += "st";
    else if(index.back() == '2')
        index += "nd";
    else if(index.back() == '3')
        index += "rd";
    else
        index += "th";
        
    return index;
}

std::string Utils::titlize(std::string title) {
    title[0] = std::toupper(title[0]);
    return title;
}

int Utils::hex(char c)
{
    return (c >= 'A') ? (c - 'A' + 10) : (c - '0');
}

std::string Utils::decodeURI(std::string const& uri) {
    std::string str;
    
    for(std::size_t i = 0; i < uri.size(); ++i)
        if(uri[i] == '%') {
            str += Utils::hex(uri[i+1]) * 16 + Utils::hex(uri[i+2]);
            i += 2;
        } else if(uri[i] == '+')
            str += ' ';
        else
            str += uri[i];
        
    return str;
}

std::map<std::string, std::string> Utils::paramsURI(std::string const& uri) {
    std::map<std::string, std::string> params_map;
    
    const char* params_str = std::strstr(uri.c_str(), "?");
    if(!params_str)
        return params_map;
    
    
    std::vector<std::string> params = Utils::split('&', params_str+1);
    for(std::string const& param : params) {
        std::vector<std::string> pair = Utils::split('=', param);
        if(pair.size() == 2)
            params_map[pair[0]] = Utils::decodeURI(pair[1]);
    }
    
    return params_map;
}

std::string const& Utils::append(std::string &str, char c) {
    if(str.back() != c)
        str += c;
    return str;
}