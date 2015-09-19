#include "publisher.hpp"

#include "utils.hpp"

Publisher* Publisher::create(int id, const std::string& name, const std::string& sort)
{
    return Factory::create<Publisher>(id, name, sort);
}