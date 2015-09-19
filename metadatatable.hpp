#ifndef __METADATATABLE_H__
#define __METADATATABLE_H__

#include <set>
#include <string>

#include "table.hpp"

class Data;
struct SearchQuery;

class MetadataTable : public Table {
public:
    MetadataTable() : Table() {}
    virtual ~MetadataTable() {}
    
    virtual std::set<Data*> fetchAll() override;
    virtual Data* fetch(int id) override;
    virtual std::set<Data*> search(SearchQuery const& query) override;
protected:
    virtual std::string searchQuery(const std::set< std::string >& query, const std::string& op) override;
};

#endif