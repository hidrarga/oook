#ifndef __AUTHOR_H__
#define __AUTHOR_H__

#include "metadata.hpp"

#include <string>

class Author : public Metadata {
    friend struct Factory;
public:
    virtual ~Author() {}
    
    virtual std::string type() const override {
        return "author";
    }
    
    static Author* create(int id, std::string const& name, std::string const& sort);
protected:
    Author(int id, std::string const& name, std::string const& sort) : Metadata(id, name, sort) {}
};

#endif