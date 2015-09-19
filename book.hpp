#ifndef __BOOK_H__
#define __BOOK_H__

#include <map>
#include <set>
#include <string>

#include "utils.hpp"
#include "data.hpp"

class Author;
class Tag;
class Publisher;
class Serie;
class Format;

class Book : public Data {
    friend struct Factory;
public:
    virtual ~Book() {}
    
    int index() const {
        return _index;
    }
    
    std::string summary() const {
        return _summary;
    }
    
    int rating() const {
        return _rating;
    }
    
    std::string path() const {
        return _path;
    }
    
    std::set<Data*> const& authors() const {
        return _authors;
    }
    
    std::set<Data*> const& tags() const {
        return _tags;
    }
    
    std::set<Data*> const& publishers() const {
        return _publishers;
    }
    
    std::set<Data*> const& series() const {
        return _series;
    }
    
    std::set<Data*> const& formats() const {
        return _formats;
    }
    
    virtual std::string type() const override {
        return "book";
    }
    
    void addAuthor(Author* author);
    void addTag(Tag* tag);
    void addPublisher(Publisher* pub);
    void addSerie(Serie* serie);
    void addFormat(Format* format);
    
    static Book* create(
        int id, 
        std::string const& name, 
        std::string const& sort,
        int index,
        std::string const& summary = std::string(),
        int rating = -1,
        std::string const& path = std::string()
    );
    
    virtual std::string renderLink() const override;
    virtual std::string renderItem(std::string const &dir) const override;
    virtual std::string render() const override;
protected:
    Book(
        int id, 
        std::string const& name, 
        std::string const& sort,
        int index,
        std::string const& summary,
        int rating,
        std::string const& path
    );
    
    int _index;
    std::string _summary;
    int _rating;
    std::string _path;
    std::set<Data*> _authors;
    std::set<Data*> _tags;
    std::set<Data*> _publishers;
    std::set<Data*> _series;
    std::set<Data*> _formats;
};

#endif