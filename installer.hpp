#ifndef __SETUP_H__
#define __SETUP_H__

#include <string>
#include <set>
#include <map>
#include <fstream>

#include "booktable.hpp"
#include "config.hpp"
#include "renderer.hpp"

#include <QtGui/QProgressDialog>

class Data;
class Dialog;

class Installer {
public:
    Installer(QDialog* parent = nullptr);
    ~Installer() {};
    
    void copy(std::set<Data*> const& books);
    
    void createHome();
    std::string createDirectory(const Data* data);
    void createDirectory(std::string const& dir);
    
    void create(std::string const& type, std::set<Data*> const& dataset = std::set<Data*>());
    void create(std::string const& type, std::map<std::string, std::set<Data*>> const& dataset);
    void create(Data const* data);
    
    template<typename C>
    void create(Data const* data, std::set<Data*, C> const& books) {
        std::string dir = createDirectory(data);
        
        createPagination(books, data->type(), dir, "../../", (data->name().empty()) ? "Unknown" : data->name());
    }
    
    template<typename C>
    void createPagination(std::set<Data*, C> const& dataset, std::string const& type, std::string const& dir, std::string const& directory = "../", std::string const& title = std::string()) {
        std::size_t offset = std::stoi(Config::get("items_per_page"));
        std::size_t pages = dataset.size() / offset + 1;
        for(std::size_t i = 0; i < pages; ++i) {
            std::string filename = (i == 0) ? "index.html" : std::to_string(i) + ".html";
        
            std::ofstream html(dir + filename);

            html << Renderer::instance().renderSection(dataset, type, title, directory, i, offset);
        
            html.close();
        }
    }
protected:
    BookTable _booktable;
    std::set<Data*> _books;
    std::map<std::string, std::set<Data*>> _metadata;
    std::map<std::string, std::set<Data*>> _formats;
    int _nformats;
    std::map<std::string, std::set<Data*>> _ratings;
    QProgressDialog _spinner;
};

#endif