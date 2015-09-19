#include "serietable.hpp"

#include "serie.hpp"

Data* SerieTable::createData(cppdb::result& res) const
{
    return Serie::create(
        res.get<int>("id"), 
        res.get<std::string>("name"), 
        res.get<std::string>("sort", "")
    );
}