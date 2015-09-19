#include "data.hpp"

#include <algorithm>

bool std::less<Data*>::operator()(Data const* left, Data const* right) const {
    return left->compare(right);
}

Data::Data(int id, std::string const& name, std::string const& sort) :
    _id(id),
    _name(name),
    _sort(sort.empty() ? name : sort),
    _lowerSort(_sort)
{
    std::transform(_lowerSort.begin(), _lowerSort.end(), _lowerSort.begin(), tolower);
}

Data::Data(int id, std::string const& name) :
    _id(id),
    _name(name),
    _sort(name),
    _lowerSort(name)
{
    std::transform(_lowerSort.begin(), _lowerSort.end(), _lowerSort.begin(), tolower);
}