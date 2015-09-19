#ifndef __DATA_H__
#define __DATA_H__

#include <map>
#include <utility>
#include <string>

class Data;

struct Factory {
    template<class T>
    static T* create(int id, std::string const& name, std::string const& sort)
    {
        static std::map<int, T*> map;
        
        auto it = map.find(id);
        
        if(it == map.end())
            it = map.insert(std::pair<int, T*>(id, new T(id, name, sort))).first;
        
        return it->second;
    }
    
    template<class T>
    static T* create(int id, std::string const& name)
    {
        static std::map<int, T*> map;
        
        auto it = map.find(id);
        
        if(it == map.end())
            it = map.insert(std::pair<int, T*>(id, new T(id, name))).first;
        
        return it->second;
    }
};

namespace std
{
    template<>
    struct less<Data*>
    {
    public:
        bool operator()(Data const* left, Data const* right) const;
    };
}

class Data
{
public:
    virtual ~Data() {}
    
    int id() const
    {
        return _id;
    }
    
    std::string name() const 
    {
        return _name;
    }
    
    std::string sort() const
    {
        return _sort;
    }
    
    std::string url() const 
    {
        return type() + "/" + std::to_string(_id);
    }
    
    virtual std::string type() const = 0;
    
    virtual bool compare(Data const* other) const {
        return _lowerSort.compare(other->_lowerSort) < 0;
    }
    
    virtual std::string  renderLink() const = 0;
    virtual std::string renderItem(std::string const& dir) const = 0;
    virtual std::string render() const = 0;
protected:
    Data(int id, const std::string& name, std::string const& sort);
    Data(int id, const std::string& name);
  
    int _id;
    std::string _name;
    std::string _sort;
    std::string _lowerSort;
};

#endif