#include "Category.hpp"

#include "../util/CategoryIdGenerator.hpp"

Category Category::create(const std::string& name, const CategoryColor color)
{
    return Category{CategoryIdGenerator::generate(), name, color};
}