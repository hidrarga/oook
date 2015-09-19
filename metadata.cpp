#include "metadata.hpp"

#include <fstream>
#include <iostream>

#include "book.hpp"
#include "utils.hpp"
#include "config.hpp"
#include "renderer.hpp"

Metadata::Metadata(int id, const std::string& name, std::string const& sort):
    Data(id, name, sort)
{
}

Metadata::Metadata(int id, const std::string& name):
    Data(id, name)
{
}

void Metadata::addBook(Book* book)
{
    _books.insert(book);
}

std::string Metadata::renderLink() const
{
    return Renderer::instance().renderLink(
        _name,
        "../../" + type() + "/" + std::to_string(_id)
    );
}

std::string Metadata::renderItem(std::string const &dir) const
{
    return Renderer::instance().renderItem(
        (!_sort.empty()) ? _sort : "Unknown", 
        std::to_string(_id),
        dir + url(), 
        dir + type() + "/icon.png",
        _books.size()
    );
}

std::string Metadata::render() const
{
    return Renderer::instance().renderSection(
        _books, 
        type(), 
        _name, 
        "../"
    );
}