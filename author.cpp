#include "author.hpp"

#include "utils.hpp"

Author* Author::create(int id, const std::string& name, const std::string& sort)
{
    return Factory::create<Author>(id, name, sort);
}