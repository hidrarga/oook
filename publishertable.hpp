#ifndef __PUBLISHERTABLE_H__
#define __PUBLISHERTABLE_H__

#include <string>
#include <cppdb/frontend.h>

#include "metadatatable.hpp"

class Data;

class PublisherTable : public MetadataTable 
{
public:
    PublisherTable() : MetadataTable() {}
    virtual ~PublisherTable() {}
    
    virtual std::string const name() const override
    {
        return "publishers";
    }
protected:
    virtual Data* createData(cppdb::result& res) const override;
};

#endif