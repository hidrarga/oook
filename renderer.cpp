#include "renderer.hpp"

#include <fstream>

#include "data.hpp"
#include "publisher.hpp"
#include "book.hpp"
#include "config.hpp"
#include "utils.hpp"

std::string Renderer::header(const std::string& directory, const std::string& active)
{
    std::string content = open("header.tpl.html");
    
    Utils::replaceAll(content, "{{title}}", Config::get("title"));
    Utils::replaceAll(content, "{{root}}", directory);
    Utils::replaceAll(content, "{{" + active + "}}", "active");

    return content;
}

std::string Renderer::footer()
{
    return open("footer.tpl.html");
}

std::string Renderer::pagination(std::size_t page, std::size_t n) {
    std::size_t offset = std::stoi(Config::get("items_per_page"));
    std::size_t pages = n / offset + 1;
    
    std::string pagination;
    
    if(pages > 1) {
        std::string content;
        
        if(page != 0) {
            content += open("pagination.first.tpl.html");
            
            std::string prev = open("pagination.prev.tpl.html");
            
            std::string index = (page-1 == 0) ? "." : std::to_string(page-1) + ".html";
            Utils::replaceAll(prev, "{{page}}", index);
            
            content += prev;
        }
        
        for(std::size_t i = 0; i < pages; ++i) {
            if(i < 2 or i >= pages-2 or (i >= page-1 and i < page+2) or (i == page-2 and i == 2) or (i == page+2 and i == pages-3)) {
                std::string current = open("pagination.link.tpl.html");
                
                std::string index = (i == 0) ? "." : std::to_string(i) + ".html";
                Utils::replaceAll(current, "{{link}}", index);
                Utils::replaceAll(current, "{{page}}", i+1);
                Utils::replaceAll(current, "{{active}}", (page == i) ? "active" : "");
                
                content += current;
            } else if((i == 3 and page-2 < i) or (i == pages-3 and page+2 > i) or i == page-2 or i == page+2)
                content += open("pagination.hellip.tpl.html");
        }
        
        if(page != pages-1) {
            std::string next = open("pagination.next.tpl.html");
            
            std::string index = std::to_string(page+1) + ".html";
            Utils::replaceAll(next, "{{page}}", index);
            
            content += next;
            
            std::string last = open("pagination.last.tpl.html");
            
            Utils::replaceAll(last, "{{page}}", pages-1);
            
            content += last;
        }
        
        pagination = open("pagination.tpl.html");
        
        Utils::replaceAll(pagination, "{{pages}}", content);
    }
    
    return pagination;
}

std::string Renderer::renderItem(
    std::string const &title, 
    std::string const &id,
    std::string const &url,
    std::string const &image,
    int count
) {
    std::string item = open("item.tpl.html");
    
    Utils::replaceAll(item, "{{title}}", title);
    Utils::replaceAll(item, "{{id}}", id);
    Utils::replaceAll(item, "{{url}}", url);
    Utils::replaceAll(item, "{{image}}", image);
    Utils::replaceAll(item, "{{count}}", (count > 0) ? "(" + std::to_string(count) + ")" : "");
    
    return item;
}

std::string Renderer::renderHomeItem(
    std::string const &type,
    int count
) {
    return renderItem(
        Utils::titlize(type),
        type,
        type,
        type + "/icon.png",
        count
    );
}

std::string Renderer::renderLink(
    std::string const& title,
    std::string const& url
) {
    if(title.empty())
        return "";
    
    std::string link = open("link.tpl.html");
    
    Utils::replaceAll(link, "{{name}}", title);
    Utils::replaceAll(link, "{{url}}", url);

    return link;

}

std::string Renderer::open(const std::string& filename) {
    auto it = _cache.find(filename);
    
    if(it == _cache.end()) {
        std::string path = Config::get("template") + "/" + filename;
        std::ifstream file(path.c_str());
    
        if(!file)
            throw std::runtime_error("Renderer: can't open template " + path);
    
        std::string content;
        std::string line;
        for(int i = 0; std::getline(file, line); ++i) {
            if(i != 0)
                content += "\n";
            content += line;
        }
        
        file.close();
        
        it = _cache.insert(std::pair<std::string, std::string>(filename, content)).first;   
    }
    
    return it->second;
}