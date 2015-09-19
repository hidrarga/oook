#ifndef __AUTHORTABLE_H__
#define __AUTHORTABLE_H__

#include <string>

#include "metadatatable.hpp"

class Data;

class AuthorTable : public MetadataTable 
{
public:
    AuthorTable() : MetadataTable() {}
    virtual ~AuthorTable() {}
    
    virtual std::string const name() const override
    {
        return "authors";
    }
protected:
    virtual Data* createData(cppdb::result & res) const override;
};

#endif