#ifndef __TAG_H__
#define __TAG_H__

#include "metadata.hpp"

#include <string>

class Tag : public Metadata {
    friend struct Factory;
public:
    virtual ~Tag() {}
    
    virtual std::string type() const override {
        return "tag";
    }
    
    static Tag* create(int id, std::string const& name);
protected:
    Tag(int id, std::string const& name) : Metadata(id, name) {}
};

#endif