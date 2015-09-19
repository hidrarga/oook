#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <set>
#include <vector>
#include <string>
#include <map>
#include "config.hpp"
#include "data.hpp"
#include "utils.hpp"

class Data;
class Book;

class Renderer {
public:
    static Renderer& instance() {
        static Renderer instance;
        return instance;
    }
    
    std::string renderItem(
        std::string const &title,
        std::string const &id,
        std::string const &url,
        std::string const &image,
        int count = -1
    );
    
    std::string renderHomeItem(
        std::string const &type,
        int count = -1
    );
    
    std::string renderLink(
        std::string const &title,
        std::string const &url
    );
    
    template<typename T>
    std::string renderSection(
        std::vector<std::set<Data*, T>> const& vector,
        std::string const& dir
    ) {
        std::string sections;
        
        for(std::set<Data*, T> const& dataset : vector) {
            if(dataset.empty())
                continue;
            
            std::string type = (*dataset.begin())->type();
            
            std::string items;
            for(Data const* data: dataset)
                items += data->renderItem(dir);
            
            std::string section = open("section.tpl.html");
            
            Utils::replaceAll(section, "{{id}}", type);
            Utils::replaceAll(section, "{{title}}", Utils::plural(Utils::titlize(type)));
            Utils::replaceAll(section, "{{items}}", items);
            
            sections += section;
        }
        return header(dir) + sections + footer();
    }
    
    template<typename T>
    std::string renderSection(
        std::map<std::string, std::set<Data*, T>> const& map,
        std::string const& type,
        std::string const& title,
        std::string const& dir,
        std::size_t page = 0,
        std::size_t offset = std::stoi(Config::get("ITEMS_PER_PAGE"))
    ) {   
        if(map.empty())
            return "";
        
        std::string items;
        
        std::size_t max = offset*page + offset;
        
        int i;
        std::map<std::string, std::set<Data*>>::const_iterator it;
        for(i = offset*page, it = map.begin(); it != map.end() and i < max; ++it, ++i) {
            items += renderItem(
                (!it->first.empty()) ? Utils::titlize(it->first) : "Unknown",
                it->first,
                dir + type + "/" + std::to_string(i),
                dir + type + "/icon.png",
                it->second.size()
            );
        }
        
        std::string section = open("section.tpl.html");
        
        Utils::replaceAll(section, "{{id}}", type);
        Utils::replaceAll(section, "{{title}}", title);
        Utils::replaceAll(section, "{{items}}", items);
        
        section += pagination(page, map.size());
        
        return header(dir, type) + section + footer();
    }
    
    template<typename T>
    std::string renderSection(
        std::set<Data*, T> const &set, 
        std::string const &type,
        std::string const& title, 
        std::string const& dir,
        std::size_t page = 0,
        std::size_t offset = std::stoi(Config::get("ITEMS_PER_PAGE"))
    ) {   
        std::string items;
        
        std::size_t max = offset*page + offset;
        
        std::size_t i;
        std::set<Data*>::const_iterator it;
        for(i = offset*page, it = std::next(set.begin(), i); it != set.end() and i < max; ++it, ++i)
            items += (*it)->renderItem(dir);
        
        std::string section = open("section.tpl.html");
        
        Utils::replaceAll(section, "{{id}}", type);
        Utils::replaceAll(section, "{{title}}", title);
        Utils::replaceAll(section, "{{items}}", items);
        
        section += pagination(page, set.size());
        
        return header(dir, type) + section + footer();
    }

    
    std::string open(const std::string& filename);
    
    std::string pagination(std::size_t page, std::size_t n);
    
    void clearCache() {
        _cache.clear();
    }
    
    std::string header(std::string const& directory = std::string(), std::string const& active = std::string());
    std::string footer();
protected:
    Renderer() {}
    
protected:
    std::map<std::string, std::string> _cache;
};

#endif