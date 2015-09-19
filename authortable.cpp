#include "authortable.hpp"

#include "author.hpp"
#include "book.hpp"

Data* AuthorTable::createData(cppdb::result& res) const
{
    Author* author = Author::create(
        res.get<int>("id"),
        res.get<std::string>("name"), 
        res.get<std::string>("sort", "")
    );
    
    return author;
}