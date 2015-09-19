#include "tag.hpp"

#include "utils.hpp"

Tag* Tag::create(int id, const std::string& name)
{
    return Factory::create<Tag>(id, name);
}