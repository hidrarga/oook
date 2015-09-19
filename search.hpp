#ifndef __SEARCH_H__
#define __SEARCH_H__

#include <vector>
#include <string>
#include <set>

class Table;
class Data;

struct SearchQuery {
    std::set<std::string> ands;
    std::set<std::string> ors;
    std::set<std::string> nots;  
};

enum SearchState {
    None = ' ',
    DoubleQuote = '"',
    BackSlash = '\\'
};

class Search {
public:
    Search();
    ~Search();
    
    std::vector<std::set<Data*>> search(std::string const& entry);
    std::vector<std::string> tokenize(std::string const& entry);
    SearchQuery group(const std::vector< std::string >& tokens);
protected:
    Table* _tables[5];
};

#endif