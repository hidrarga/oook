#include "metadatatable.hpp"

#include "search.hpp"
#include "author.hpp"

std::set<Data*> MetadataTable::fetchAll()
{
    std::vector<std::string> fields{
        "id", 
        "name", 
        "sort"
    };
    
    return Table::fetchAll(fields);
}

Data* MetadataTable::fetch(int id)
{
    std::vector<std::string> fields{"id", "name", "sort"};
    
    return Table::fetch(id, fields);
}

std::string MetadataTable::searchQuery(const std::set<std::string>& query, 
                                   const std::string& op)
{
    std::vector<std::string> fields{"name"};
    
    return Table::searchQuery(query, op, fields);
}

std::set<Data*> MetadataTable::search(SearchQuery const& query)
{
    std::vector<std::string> fields{"id", "name", "sort"};
    
    return Table::search(query, fields);
}