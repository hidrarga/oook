#ifndef __PUBLISHER_H__
#define __PUBLISHER_H__

#include "metadata.hpp"

#include <string>

class Publisher : public Metadata {
    friend struct Factory;
public:
    virtual ~Publisher() {}
    
    virtual std::string type() const override {
        return "publisher";
    }
    
    static Publisher* create(int id, std::string const& name, std::string const& sort);
protected:
    Publisher(int id, std::string const& name, std::string const& sort) : Metadata(id, name, sort) {}
};

#endif