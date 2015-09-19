#include "search.hpp"

#include <iostream>

#include "booktable.hpp"
#include "authortable.hpp"
#include "publishertable.hpp"
#include "serietable.hpp"
#include "tagtable.hpp"

#include "data.hpp"

#include "utils.hpp"
#include "config.hpp"
#include "configparser.hpp"
#include "renderer.hpp"

#include <fastcgi/fcgio.h>


Search::Search() :
_tables{new BookTable(), new AuthorTable(), new PublisherTable(), new SerieTable(), new TagTable()}
{
}

Search::~Search() {
    for(Table* table: _tables)
        delete table;
}

std::vector< std::set< Data* > > Search::search(const std::string& entry) {
    std::vector<std::set<Data*>> results;
    if(!entry.empty()) {
        std::vector<std::string> tokens = tokenize(entry);
        
        if(!tokens.empty()) {
            SearchQuery query = group(tokens);
            
            for(Table* table: _tables)
                results.push_back(table->search(query));
        }   
    }
    
    return results;
}

std::vector<std::string> Search::tokenize(std::string const& entry) {
    std::vector<std::string> tokens;
    
    SearchState state = None, oldState = None;
    
    int start = 0;
    int length = 0;
    for(std::size_t i = 0; i < entry.size(); ++i, ++length) {
        char c = entry[i];
        
        if(c == DoubleQuote and state != BackSlash) {
            if(state == DoubleQuote)
                state = None;
            else
                state = DoubleQuote;
            
            if(length > 0)
                tokens.push_back(entry.substr(start, length));
            
            start = i+1;
            length = -1;
        } else {
            if(state == BackSlash)
                state = oldState;
            
            if(c == None and state == None) {
                if(length > 0)
                    tokens.push_back(entry.substr(start, length));
                
                start = i+1;
                length = -1;
            } else if(c == BackSlash) {
                oldState = state;
                state = BackSlash;
            }
        }
    }
    
    if(length > 0)
        tokens.push_back(entry.substr(start, length));
    
    /* Post-Processing */
        
    for(auto it = tokens.begin(); it != tokens.end(); ++it) {
        if((*it) == "+" or (*it) == "-") {
            std::string token = (*it);
            tokens.erase(it);
            
            if(it != tokens.end())
                (*it) = token + (*it);
            
            --it;
        } else
            Utils::replace((*it), "\\\"", "\"");
    }
    
    return tokens;
}

SearchQuery Search::group(std::vector<std::string> const& tokens) {
    SearchQuery query;
    
    for(std::string const& token : tokens) {
        switch(token[0]) {
            case '+':
                query.ands.insert(token.substr(1, token.size()));
                break;
            case '-':
                query.nots.insert(token.substr(1, token.size()));
                break;
            default:
                query.ors.insert(token);
                break;
        }
    }
    
    return query;
}


int main(int argc, char **argv) {
    ConfigParser().read("oook.conf");
    
    std::streambuf* cin_streambuf  = std::cin.rdbuf();
    std::streambuf* cout_streambuf = std::cout.rdbuf();
    std::streambuf* cerr_streambuf = std::cerr.rdbuf();
    
    FCGX_Init();
    
    FCGX_Request request;
    FCGX_InitRequest(&request,0,0);
    
    Search search;
    
    while(FCGX_Accept_r(&request) == 0)
    {   
        fcgi_streambuf cin_fcgi_streambuf(request.in);
        fcgi_streambuf cout_fcgi_streambuf(request.out);
        fcgi_streambuf cerr_fcgi_streambuf(request.err);
        
        std::cin.rdbuf(&cin_fcgi_streambuf);
        std::cout.rdbuf(&cout_fcgi_streambuf);
        std::cerr.rdbuf(&cerr_fcgi_streambuf);

        std::cout << "Content-type: text/html" << std::endl << std::endl;
        
        std::map<std::string, std::string> params = Utils::paramsURI(
            FCGX_GetParam("REQUEST_URI", request.envp)
        );
        
        std::vector<std::set<Data*>> results;
        
        try {
            results = search.search(params.at("q"));
        } catch(std::exception const& e) {
            std::cerr << e.what() << std::endl;
        }
        
        std::cout << Renderer::instance().renderSection(results, "../") << std::endl;
    }
    
    std::cin.rdbuf(cin_streambuf);
    std::cout.rdbuf(cout_streambuf);
    std::cerr.rdbuf(cerr_streambuf);
    
    return 0;
}
