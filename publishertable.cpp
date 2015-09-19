#include "publishertable.hpp"

#include "publisher.hpp"

Data* PublisherTable::createData(cppdb::result & res) const
{
    return Publisher::create(
        res.get<int>("id"), 
        res.get<std::string>("name"), 
        res.get<std::string>("sort", "")
    );
}