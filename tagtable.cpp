#include "tagtable.hpp"

#include <vector>

#include "search.hpp"
#include "tag.hpp"

Data* TagTable::createData(cppdb::result& res) const
{
    return Tag::create(
        res.get<int>("id"), 
        res.get<std::string>("name")
    );
}

std::set<Data*> TagTable::fetchAll()
{
    std::vector<std::string> fields{"id", "name"};
    
    return Table::fetchAll(fields);
}

Data* TagTable::fetch(int id)
{
    std::vector<std::string> fields{"id", "name"};
    
    return Table::fetch(id, fields);
}

std::string TagTable::searchQuery(const std::set<std::string>& query, 
                                   const std::string& op)
{
    std::vector<std::string> fields{"name"};
    
    return Table::searchQuery(query, op, fields);
}

std::set<Data*> TagTable::search(SearchQuery const& query)
{
    std::vector<std::string> fields{"id", "name"};
    
    return Table::search(query, fields);
}