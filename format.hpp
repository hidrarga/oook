#ifndef __FORMAT_H__
#define __FORMAT_H__

#include <string>

#include "metadata.hpp"

class Format : public Metadata {
    friend struct Factory;
public:
    virtual ~Format() {}
    
    std::string format() const {
        return _format;
    }
    
    int size() const {
        return _size;
    }
    
    virtual std::string type() const override {
        return "format";
    }
    
    virtual std::string renderLink() const override;
    
    static Format* create(int id, std::string const& name, std::string const& format, int size);
protected:
    Format(int id, std::string const& name, std::string const& format, int size);
    
    std::string _format;
    int _size;
};

#endif