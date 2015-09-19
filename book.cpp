#include "book.hpp"

#include <fstream>
#include <algorithm>

#include "author.hpp"
#include "tag.hpp"
#include "publisher.hpp"
#include "serie.hpp"
#include "format.hpp"

#include "utils.hpp"
#include "config.hpp"
#include "renderer.hpp"

Book::Book(
    int id, 
    std::string const& name, 
    std::string const& sort,
    int index,
    std::string const& summary,
    int rating,
    std::string const& path
) :
    Data(id, name, sort),
    _index(index),
    _summary(summary),
    _rating(rating),
    _path(path),
    _authors(),
    _tags(),
    _publishers(),
    _series(),
    _formats()
{
}

void Book::addAuthor(Author* author) {
    _authors.insert(author);
}

void Book::addTag(Tag* tag) {
    _tags.insert(tag);
}

void Book::addPublisher(Publisher* pub) {
    _publishers.insert(pub);
}

void Book::addSerie(Serie* serie) {
    _series.insert(serie);
}

void Book::addFormat(Format* format) {
    _formats.insert(format);
}

Book* Book::create(
    int id, 
    const std::string& name, 
    const std::string& sort,
    int index,
    std::string const& summary,
    int rating,
    std::string const& path
)
{   
    static std::map<int, Book*> books;
        
    auto it = books.find(id);
    
    if(it == books.end())
        it = books.insert(std::pair<int, Book*>(id, new Book(id, name, sort, index, summary, rating, path))).first;
    else {
        Book* book = it->second;
        if(!summary.empty())
            book->_summary = summary;
        if(rating >= 0)
            book->_rating = rating;
        if(!path.empty())
            book->_path = path;
    }
        
    return it->second;
}

std::string Book::renderLink() const
{
    return "";
}

std::string Book::renderItem(std::string const &dir) const
{
    std::string out = Renderer::instance().open("item.book.tpl.html");
    
    Utils::replaceAll(out, "{{id}}", std::to_string(_id));
    Utils::replaceAll(out, "{{title}}", (!_name.empty()) ? _name : "Unknown"); 
    Utils::replaceAll(out, "{{url}}", dir + url());
    Utils::replaceAll(out, "{{legend}}", Utils::join(" & ", _authors));
    
    return out;
}

std::string Book::render() const
{
    std::string downloads;
    for(Data const* format: _formats)
        downloads += format->renderLink();
    
    std::string tags;
    for(auto it = _tags.begin(); it != _tags.end(); ++it) {
        if(it != _tags.begin())
            tags += ", ";
        tags += (*it)->renderLink();
    }
    
    std::string pubs;
    for(auto it = _publishers.begin(); it != _publishers.end(); ++it) {
        if(it != _publishers.begin())
            pubs += ", ";
        pubs += (*it)->renderLink();
    }
        
    std::string authors;
    for(auto it = _authors.begin(); it != _authors.end(); ++it) {
        if(it != _authors.begin())
            authors += " & ";
        authors += (*it)->renderLink();
    }
    
    std::string series;
    for(auto it = _series.begin(); it != _series.end(); ++it) {
        if(it != _series.begin())
            series += " & ";
        series += (*it)->renderLink();
    }
    
    if(!series.empty())
        series = Utils::ordinal(_index) + " book in the " + series + "'s series";
    
    std::string out = Renderer::instance().open("book.tpl.html");
    
    Utils::replaceAll(out, "{{downloads}}", downloads);
    Utils::replaceAll(out, "{{tags}}", tags);
    Utils::replaceAll(out, "{{authors}}", authors);
    Utils::replaceAll(out, "{{publishers}}", pubs);
    Utils::replaceAll(out, "{{series}}", series);
    Utils::replaceAll(out, "{{summary}}", _summary);
    Utils::replaceAll(out, "{{rating-1}}", (_rating/2 > 0) ? "" : "-empty");
    Utils::replaceAll(out, "{{rating-2}}", (_rating/2 > 1) ? "" : "-empty");
    Utils::replaceAll(out, "{{rating-3}}", (_rating/2 > 2) ? "" : "-empty");
    Utils::replaceAll(out, "{{rating-4}}", (_rating/2 > 3) ? "" : "-empty");
    Utils::replaceAll(out, "{{rating-5}}", (_rating/2 > 4) ? "" : "-empty");
    Utils::replaceAll(out, "{{name}}", _name);
    
    return Renderer::instance().header("../../", type()) + out + Renderer::instance().footer();
}