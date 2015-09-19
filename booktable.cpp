#include "booktable.hpp"

#include "search.hpp"
#include "book.hpp"
#include "author.hpp"
#include "tag.hpp"
#include "publisher.hpp"
#include "serie.hpp"
#include "format.hpp"

Data* BookTable::createData(cppdb::result& res) const
{
    std::string summary;
    int rating = -1;
    std::string path;
    
    try {
        summary = res.get<std::string>("summary", "");
    } catch(cppdb::invalid_column const& e) {}
    try {
        rating = res.get<int>("rating", rating);
    } catch(cppdb::invalid_column const& e) {}
    try {
        path = res.get<std::string>("path", path);
    } catch(cppdb::invalid_column const& e) {}
    
    Book* book = Book::create(
        res.get<int>("id"), 
        res.get<std::string>("title"), 
        res.get<std::string>("sort", ""),
        res.get<int>("series_index"),
        summary,
        rating, 
        path
    );
    
    Author* author = Author::create(
        res.get<int>("author_id", 0), 
        res.get<std::string>("author", ""), 
        res.get<std::string>("author_sort", "")
    ); 
    book->addAuthor(author);
    author->addBook(book);
    
    try {
        Tag* tag = Tag::create(
            res.get<int>("tag_id", 0), 
            res.get<std::string>("tag", "")
        ); 
        book->addTag(tag);
        tag->addBook(book);
    } catch(cppdb::cppdb_error const& e) {}
    
    try {
        Publisher* pub = Publisher::create(
            res.get<int>("publisher_id", 0), 
            res.get<std::string>("publisher", ""),
            res.get<std::string>("publisher_sort", "")
        ); 
        book->addPublisher(pub);
        pub->addBook(book);
    } catch(cppdb::cppdb_error const& e) {}
    
    try {
        Serie* serie = Serie::create(
            res.get<int>("serie_id", 0), 
            res.get<std::string>("serie", ""),
            res.get<std::string>("serie_sort", "")
        ); 
        book->addSerie(serie);
        serie->addBook(book);
    } catch(cppdb::cppdb_error const& e) {}
    
    try {
        Format* format = Format::create(
            res.get<int>("file_id"),
            res.get<std::string>("filename"),
            res.get<std::string>("format"),
            res.get<int>("filesize")
        );
        book->addFormat(format);
        format->addBook(book);
    } catch(cppdb::cppdb_error const& e) {}
    
    return book;
}

std::set<Data*> BookTable::fetchAll()
{
    std::vector<std::string> fields{
        "id", 
        "title", 
        "sort", 
        "series_index", 
        "a.name author", 
        "a.id author_id", 
        "a.sort author_sort"
    };
    
    std::vector<Join> joins;
    joins.push_back(createJoin("books_authors_link ba", "book", "id"));
    joins.push_back(createJoin("authors a", "id", "author", "ba"));
    
    return Table::fetchAll(fields, joins);
}

Data* BookTable::fetch(int id)
{
    std::vector<std::string> fields{
        "id", 
        "title", 
        "sort", 
        "path",
        "series_index",
        "d.format", 
        "d.id file_id", 
        "d.name filename", 
        "d.uncompressed_size filesize", 
        "c.text summary", 
        "r.rating",
        "a.name author", 
        "a.id author_id", 
        "a.sort author_sort",
        "k.name tag",
        "k.id tag_id",
        "p.name publisher",
        "p.id publisher_id",
        "p.sort publisher_sort",
        "s.name serie",
        "s.id serie_id",
        "s.sort serie_sort"
    };
    
    std::vector<Join> joins;
    joins.push_back(createJoin("books_authors_link ba", "book", "id"));
    joins.push_back(createJoin("authors a", "id", "author", "ba"));
    
    joins.push_back(createJoin("books_ratings_link br", "book", "id"));
    joins.push_back(createJoin("ratings r", "id", "rating", "br"));
    
    joins.push_back(createJoin("books_tags_link bk", "book", "id"));
    joins.push_back(createJoin("tags k", "id", "tag", "bk"));
    
    joins.push_back(createJoin("books_publishers_link bp", "book", "id"));
    joins.push_back(createJoin("publishers p", "id", "publisher", "bp"));
    
    joins.push_back(createJoin("books_series_link bs", "book", "id"));
    joins.push_back(createJoin("series s", "id", "series", "bs"));
    
    joins.push_back(createJoin("comments c", "book", "id"));
    joins.push_back(createJoin("data d", "book", "id"));
    
    return Table::fetch(id, fields, joins);
}

std::string BookTable::searchQuery(const std::set< std::string >& query, const std::string& op)
{
    std::vector<std::string> fields{
        "title",
        "c.text"
    };
    
    std::vector<Join> joins;
    joins.push_back(createJoin("comments c", "book", "id"));
    
    return Table::searchQuery(query, op, fields, joins);
}

std::set<Data*> BookTable::search(SearchQuery const& query)
{
    std::vector<std::string> fields{
        "id", 
        "title", 
        "sort", 
        "series_index",
        "a.name author", 
        "a.id author_id", 
        "a.sort author_sort"
    };
    
    std::vector<Join> joins;
    joins.push_back(createJoin("books_authors_link ba", "book", "id"));
    joins.push_back(createJoin("authors a", "id", "author", "ba"));
    
    return Table::search(query, fields, joins);
}