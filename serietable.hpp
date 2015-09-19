#ifndef __SERIETABLE_H__
#define __SERIETABLE_H__

#include <string>

#include "metadatatable.hpp"

class Data;

class SerieTable : public MetadataTable 
{
public:
    SerieTable() : MetadataTable() {}
    virtual ~SerieTable() {}
    
    virtual std::string const name() const override
    {
        return "series";
    }
protected:
    virtual Data* createData(cppdb::result & res) const override;
};

#endif