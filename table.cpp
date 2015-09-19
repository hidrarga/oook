#include "table.hpp"

#include <iostream>
#include <algorithm>
#include <cstring>

#include "config.hpp"
#include "utils.hpp"
#include "search.hpp"


Table::Table() : 
    db("sqlite3:db=" + Config::get("DATABASE") + ";mode=readonly"),
    _binds()
{
}

std::set<Data*> Table::fetchAll(std::vector< std::string >& fields, const std::vector< Join >& joins)
{
    std::set<Data*> results;
    
    std::string alias = "t";
    std::string sql;
    
    for(std::string &field : fields)
        treatField(field, alias);
    
    sql = "SELECT " + Utils::join(", ", fields) + 
        " FROM " + name() + ' ' + alias;

    for(Join const& join : joins)
        sql += " " + treatJoin(join, alias);

    cppdb::result res = db << sql;
    
    while(res.next())
        results.insert(createData(res));
    
    return results;
}

Data* Table::fetch(int id, std::vector< std::string >& fields, const std::vector< Join >& joins)
{
    std::set<Data*> results;
    
    std::string alias = "t";
    std::string sql;
    
    for(std::string &field : fields)
        treatField(field, alias);
    
    sql = "SELECT " + Utils::join(", ", fields) + 
        " FROM " + name() + ' ' + alias;

    for(Join const& join : joins)
        sql += ' ' + treatJoin(join, alias);
    
    sql += " WHERE " + alias + ".id = ?";
    
    cppdb::result res = db << sql << id;
    
    while(res.next())
        results.insert(createData(res));
    return (!results.empty()) ? *results.begin() : nullptr;
}

std::set<Data*> Table::search(SearchQuery const& query, 
                              std::vector<std::string> & fields, 
                              std::vector<Join> const& joins) {
    std::set<Data*> results;
    _binds.clear();
    
    std::string alias = "t";
    std::string sql;
    std::vector<std::string> conditions;
    
    for(std::string &field : fields)
        treatField(field, alias);
    
    sql = "SELECT " + Utils::join(", ", fields) +
        " FROM " + name() + ' ' + alias;
        
    for(Join const& join : joins)
        sql += ' ' + treatJoin(join, alias);
    
    sql += " WHERE ";
    
    if(!query.ands.empty())
        conditions.push_back(alias + ".id IN (" + searchIntersection(query.ands) + ")");
    if(!query.ors.empty())
        conditions.push_back(alias + ".id IN (" + searchUnion(query.ors) + ")");
    if(!query.nots.empty())
        conditions.push_back(alias + ".id NOT IN (" + searchUnion(query.nots) + ")");
    
    sql += Utils::join(" AND ", conditions);
    
    cppdb::statement stmt = db << sql;
    
    for(std::string const& bind : _binds)
        stmt.bind(bind);
    
    cppdb::result res = stmt;
    
    while(res.next())
        results.insert(createData(res));
    
    return results;
}

Join Table::createJoin(const std::string& tableFirst, const std::string& idFirst, 
                const std::string& idSecond, const std::string& tableSecond) const
{
    Join join;
    
    join.tableFirst = tableFirst;
    join.idFirst = idFirst;
    join.idSecond = idSecond;
    join.tableSecond = tableSecond;
    
    return join;
}

std::string Table::treatField(std::string& field, const std::string& table) const
{
    if(!strstr(field.c_str(), "."))
        field = table + '.' + field;
    
    return field;
}

std::string Table::treatJoin(Join join, const std::string& table) const
{
    join.tableSecond = (!join.tableSecond.empty()) ? join.tableSecond : table;
    
    treatField(join.idFirst, alias(join.tableFirst));
    treatField(join.idSecond, join.tableSecond);
    
    return "LEFT JOIN " + join.tableFirst + " ON " + join.idFirst + " = " + join.idSecond;
}

std::string Table::alias(std::string const& table) const 
{
    std::vector<std::string> list = Utils::split(' ', table);
    return (list.size() == 2) ? list[1] : list[0];
}

std::string Table::searchQuery(std::set<std::string> const& query, 
                               const std::string& op, 
                               std::vector<std::string>& fields,
                               std::vector<Join> const& joins)
{
    std::string alias = "s";
    std::string sql;
    
    for(std::string & field : fields)
        treatField(field, alias);
    
    sql = "SELECT " + alias + ".id FROM " + name() + " " + alias;
    
    for(Join const& join : joins)
        sql += ' ' + treatJoin(join, alias);
    
    sql += " WHERE ";
    
    for(auto it_q = query.begin(); it_q != query.end(); ++it_q) {
        if(it_q != query.begin())
            sql += " " + op + " ";
        
        sql += "(";
        
        for(auto it_f = fields.begin(); it_f != fields.end(); ++it_f) {
            if(it_f != fields.begin())
                sql += " OR ";
            
            sql += (*it_f) + " LIKE ?";
            
            _binds.push_back("%" + (*it_q) + "%");
        }
        
        sql += ")";
    }
    
    return sql;
}

std::string Table::searchIntersection(const std::set< std::string >& query)
{
    return searchQuery(query, "AND");
}

std::string Table::searchUnion(const std::set< std::string >& query)
{
    return searchQuery(query, "OR");
}
