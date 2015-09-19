#ifndef __METADATA_H__
#define __METADATA_H__

#include "data.hpp"

#include <string>
#include <set>

class Book;

class Metadata : public Data {
public:
    virtual ~Metadata() {}
    
    std::set<Data*> const& books() const 
    {
        return _books;
    }
    
    void addBook(Book* book);
    
    virtual std::string renderLink() const override;
    virtual std::string renderItem(std::string const &dir) const override;
    virtual std::string render() const override;
protected:
    Metadata(int id, std::string const& name, std::string const&sort);
    Metadata(int id, std::string const& name);
    
    std::set<Data*> _books;
};
#endif