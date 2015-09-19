#ifndef __TABLE_H__
#define __TABLE_H__

#include <string>
#include <map>
#include <set>
#include <vector>

#include <cppdb/frontend.h>

#include "data.hpp"

struct SearchQuery;

struct Join {
    std::string tableFirst;
    std::string idFirst;
    std::string tableSecond;
    std::string idSecond;
};

class Table 
{
public:
    Table();
    virtual ~Table() {}
    
    virtual std::string const name() const = 0;
    
    virtual std::set<Data*> fetchAll() = 0;
    virtual Data* fetch(int id) = 0;
    //virtual Data* fetch(int id, std::vector< std::string >& fields, const std::vector< Join >& joins) = 0;
    virtual std::set<Data*> search(SearchQuery const& query) = 0;
protected:
    virtual Data* createData(cppdb::result& res) const = 0;
    Join createJoin(std::string const& tableFirst, std::string const& idFirst, 
                    std::string const& idSecond, std::string const& tableSecond = std::string()) const;
                    
    std::string treatField(std::string& field, const std::string& table) const;
    std::string treatJoin(Join join, const std::string& table) const;
    std::string alias(std::string const& table) const;
    
    virtual std::string searchQuery(std::set<std::string> const& query,
                                    std::string const& op) = 0;
                                    
    std::string searchQuery(const std::set< std::string >& query, 
                            const std::string& op, 
                            std::vector< std::string >& fields,
                            const std::vector< Join >& joins = std::vector<Join>());
    std::string searchIntersection(const std::set< std::string >& query);
    std::string searchUnion(const std::set< std::string >& query);
    
    
    std::set<Data*> fetchAll(std::vector< std::string >& fields, 
                                          const std::vector< Join >& joins = std::vector<Join>());
    Data* fetch(int id, std::vector< std::string >& fields, 
                const std::vector< Join >& joins = std::vector<Join>());
    std::set<Data*> search(const SearchQuery& query, 
                                        std::vector< std::string >& fields, 
                                        const std::vector< Join >& joins = std::vector<Join>());
protected:
    cppdb::session db;
    std::set<Data*> _results;
    std::vector<std::string> _binds;
};

#endif