#ifndef __SERIE_H__
#define __SERIE_H__

#include "metadata.hpp"

#include <string>

class Serie : public Metadata {
    friend struct Factory;
public:
    virtual ~Serie() {}
    
    virtual std::string type() const override {
        return "series";
    }
    
    static Serie* create(int id, std::string const& name, std::string const& sort);
protected:
    Serie(int id, std::string const& name, std::string const& sort) : Metadata(id, name, sort) {}
};

struct comparator_series {
    bool operator() (Data const* data, Data const* other);
};

#endif