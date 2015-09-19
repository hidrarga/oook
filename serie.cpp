#include "serie.hpp"

#include "book.hpp"

Serie* Serie::create(int id, const std::string& name, const std::string& sort)
{
    return Factory::create<Serie>(id, name, sort);
}

bool comparator_series::operator()(const Data* data, const Data* other)
{
    int index1 = static_cast<Book const*>(data)->index();
    int index2 = static_cast<Book const*>(other)->index();
    
    if(index1 == index2)
        return data->compare(other);
    else
        return index1 < index2;
}