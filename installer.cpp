#include "installer.hpp"

#include <iostream>
#include <fstream>

#include <boost/filesystem.hpp>

//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>

#include <QtCore>
#include <QtGui>
#include <QApplication>
#include <QtGui/QGridLayout>

#include "config.hpp"
#include "configparser.hpp"
#include "utils.hpp"
#include "data.hpp"
#include "book.hpp"
#include "author.hpp"
#include "serie.hpp"
#include "format.hpp"

#include "booktable.hpp"
#include "renderer.hpp"

#include "filesystemutils.hpp"

Installer::Installer(QDialog* parent) :
_booktable(),
_books(_booktable.fetchAll()),
_metadata(),
_formats(), 
_nformats(0),
_ratings(),
_spinner(parent)
{
    // First, we create the destination directory if it doesn't exist yet
    createDirectory(Config::get("catalog"));
    _metadata["author"] = std::set<Data*>();
    _metadata["tag"] = std::set<Data*>();
    _metadata["publisher"] = std::set<Data*>();
    _metadata["series"] = std::set<Data*>();
    
    for(Data const* data: _books) {
        // For each book, we retrieve metadata related to the book
        Book* book = static_cast<Book*>(_booktable.fetch(data->id()));
        
        _metadata["author"].insert(book->authors().begin(), book->authors().end());
        _metadata["tag"].insert(book->tags().begin(), book->tags().end());
        _metadata["publisher"].insert(book->publishers().begin(), book->publishers().end());
        _metadata["series"].insert(book->series().begin(), book->series().end());
        
        for(Data const* data: book->formats()) {
            // A book can have several formats (epub, pdf, ...) ; we add each book's format to its list
            Format const* format = static_cast<Format const*>(data);
            
            _formats[format->format()].insert(format->books().begin(), format->books().end());
        }
        
        // TODO find a better way to do it
        int rating = book->rating()/2;
        std::string ratingStr;
        if(book->rating() >= 0) 
            ratingStr = std::to_string(rating) + Utils::plural(" star", rating);
        _ratings[ratingStr].insert(book);
        
        // Usefull for copying data later
        _nformats += book->formats().size();
    }
    
    _spinner.setCancelButton(nullptr);
    _spinner.setRange(0, _nformats);
    _spinner.setValue(0);
    _spinner.setFixedSize(_spinner.size());
    _spinner.setModal(true);
    
    // Create HTML pages (for sections)
    create("book", _books);
    for(auto const& pair: _metadata)
        create(pair.first, pair.second);
    create("format", _formats);
    create("rating", _ratings);
    create("search");
    
    // Create HTML pages (for each book details)
    for(Data const* data: _books)
        create(data);
    // Create HTML pages (we list all books for each metadata)
    for(auto const& pair: _metadata)
        for(Data const* data: pair.second) {
            Metadata const* metadata = static_cast<Metadata const*>(data);
            
            if(pair.first == "series") {
                std::set<Data*, comparator_series> books;
                books.insert(metadata->books().begin(), metadata->books().end());
                
                create(data, books);
            } else
                create(data, metadata->books());
        }
        
    createHome();
    
    FileSystemUtils::copyDirectory(Config::get("template"), Config::get("catalog") + "template/", true, false);
    FileSystemUtils::copyDirectory(Config::get("template") + "css/", Config::get("catalog") + "css/", true, false);
    FileSystemUtils::copyFile(Config::get("template"), Config::get("catalog"), "bower.json", true);
    
    FileSystemUtils::copyFile(".", Config::get("catalog") + "search/", "search.cgi", true);
    FileSystemUtils::copyFile(Config::get("database"), Config::get("catalog") + "metadata.db", true);
    
    copy(_books);
    
    std::string catalog = Config::get("catalog");
    std::string items_per_page = Config::get("items_per_page");
    
    Config::clear();
    
    Config::set("template", "../template/");
    Config::set("database", "../metadata.db");
    Config::set("items_per_page", items_per_page);
    
    ConfigParser().write(catalog + "search/oook.conf");
}

void Installer::createHome() 
{
    std::map<std::string, int> items;
    
    items["book"] = _books.size();
    for(auto const& pair: _metadata)
        items[pair.first] = pair.second.size();
    items["format"] = _formats.size();
    items["rating"] = _ratings.size();
    
    std::ofstream html(Config::get("catalog") + "index.html");
    
    html << Renderer::instance().header();
    
    for(auto const& pair: items)
        html << Renderer::instance().renderHomeItem(
            pair.first,
            pair.second
        );
    
    html << Renderer::instance().renderHomeItem("search");
    
    html << Renderer::instance().footer();
    
    html.close();
}

void Installer::create(std::string const& type, std::set<Data*> const& dataset) 
{
    std::string dir(Config::get("catalog") + type + "/");

    createDirectory(dir);

    createPagination(dataset, type, dir);

    // icon
    FileSystemUtils::copyFile(Config::get("template") + "icon/" + type + ".png", dir + "icon.png", true);
}

void Installer::create(std::string const& type, std::map<std::string, std::set<Data*>> const& dataset) 
{
    std::string dir(Config::get("catalog") + type + "/");

    createDirectory(dir);

    std::ofstream html(dir + "index.html");
    
    html << Renderer::instance().renderSection(dataset, type, "", "../");
    
    html.close();
    
    int i = 0;
    for(auto const& pair: dataset) {
        std::string subdir(dir + std::to_string(i) + "/");

        createDirectory(subdir);
        
        createPagination(pair.second, type, subdir, "../../", Utils::titlize(pair.first));
        
        ++i;
    }
    
    // icon
    FileSystemUtils::copyFile(Config::get("template") + "icon/" + type + ".png", dir + "icon.png", true);
}

void Installer::create(Data const* data) {
    std::string dir = createDirectory(data);
    
    std::ofstream html(dir + "index.html");
    
    html << data->render();
    
    html.close();
}

void Installer::createDirectory(std::string const& dir) {
    boost::filesystem::create_directory(dir);
    
    if(!boost::filesystem::is_directory(dir))
        throw std::runtime_error("Error: can't create directory " + dir);
}

std::string Installer::createDirectory(Data const* data) {
    std::string dir(Config::get("catalog") + data->type() + "/" + std::to_string(data->id()) + "/");
    
    createDirectory(dir);
    
    return dir;
}

void Installer::copy(std::set<Data*> const& books)
{
    std::vector<std::string> mediumSize = Utils::split('x', Config::get("cover_size"));
    
    int mediumSizeL = std::stoi(mediumSize.at(0));
    int mediumSizeH = std::stoi(mediumSize.at(1));
    
    std::vector<std::string> thumbnailSize = Utils::split('x', Config::get("thumbnail_size"));
    
    int thumbnailSizeL = std::stoi(thumbnailSize.at(0));
    int thumbnailSizeH = std::stoi(thumbnailSize.at(1));
    
    std::size_t i = 0;
    for(Data const* data: books) {
        Book const* book = static_cast<Book const*>(data);
        
        std::string src = Config::get("calibre") + "/" + book->path() + "/";
        std::string dst = Config::get("catalog") + "book/" + std::to_string(book->id()) + "/";
        
        // copy ebooks
        for(Data const* data: book->formats()) {
            Format const* format = static_cast<Format const*>(data);
            
            ++i;
            _spinner.setValue(i);
            QApplication::processEvents();
            
            try {
                FileSystemUtils::copyFile(src, dst, format->name() + "." + format->format(), false);
            } catch(std::exception const& e) { }
        }
        
        // copy cover
        try {
            FileSystemUtils::copyFile(src, dst, "cover.jpg", false);
            
            QImage cover((dst + "cover.jpg").c_str());
            
            QSize medium_size(mediumSizeL, mediumSizeH);
            QSize thumbnail_size(thumbnailSizeL, thumbnailSizeH);
            
            QImage medium = cover.scaled(medium_size, Qt::KeepAspectRatio);
            QImage thumbnail = cover.scaled(thumbnail_size, Qt::KeepAspectRatio);
            
            medium.save((dst + "medium.jpg").c_str()); 
            thumbnail.save((dst + "thumbnail.jpg").c_str());
        } catch(std::exception const& e) { }
    }
    
    if(_nformats > 0)
        std::cout << "Copied " << i << "/" << _nformats << " ebooks." << std::endl;
}