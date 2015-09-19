#include "format.hpp"

#include <algorithm>

#include "utils.hpp"
#include "config.hpp"
#include "renderer.hpp"

Format::Format(int id, std::string const& name, std::string const& format, int size) :
    Metadata(id, name, format),
    _format(format),
    _size(size)
{
    std::transform(_format.begin(), _format.end(), _format.begin(), tolower);
}

std::string Format::renderLink() const
{
    if(_name.empty())
        return "";
    
    std::string content = Renderer::instance().open("link.download.tpl.html");
    
    Utils::replaceAll(content, "{{url}}", _name + "." + _format);
    Utils::replaceAll(content, "{{format}}", _format);
        
    return content;
}

Format* Format::create(int id, const std::string& name, const std::string& format, int size)
{
    static std::map<int, Format*> formats;
        
    auto it = formats.find(id);
    
    if(it == formats.end())
        it = formats.insert(std::pair<int, Format*>(id, new Format(id, name, format, size))).first;
    
    return it->second;
}
