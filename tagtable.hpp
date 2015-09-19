#ifndef __TAGTABLE_H__
#define __TAGTABLE_H__

#include <cppdb/frontend.h>
#include <string>
#include <set>

#include "metadatatable.hpp"

class Data;
struct SearchQuery;

class TagTable : public MetadataTable 
{
public:
    TagTable() : MetadataTable() {}
    virtual ~TagTable() {}
    
    virtual std::string const name() const override
    {
        return "tags";
    }
    
    virtual std::set<Data*> fetchAll() override;
    virtual Data* fetch(int id) override;
    virtual std::set<Data*> search(SearchQuery const& query) override;
protected:
    virtual std::string searchQuery(const std::set< std::string >& query, const std::string& op) override;
    virtual Data* createData(cppdb::result& res) const override;
};

#endif