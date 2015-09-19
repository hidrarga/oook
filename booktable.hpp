#ifndef __BOOKTABLE_H__
#define __BOOKTABLE_H__

#include <string>
#include <set>

#include "table.hpp"

class Data;
class SearchQuery;

class BookTable : public Table 
{
public:
    BookTable() : Table() {}
    virtual ~BookTable() {}
    
    virtual std::string const name() const override
    {
        return "books";
    }
    
    virtual std::set<Data*> fetchAll() override;
    virtual Data* fetch(int id) override;
    virtual std::set<Data*> search(const SearchQuery& query) override;
protected:
    virtual std::string searchQuery(const std::set< std::string >& query, const std::string& op) override;
    Data* createData(cppdb::result & res) const override;
};

#endif