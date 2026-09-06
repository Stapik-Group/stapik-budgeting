#pragma once

#include <string>

#include "CategoryColor.hpp"

struct Category
{
    std::string id;
    std::string name;
    CategoryColor color = CategoryColor::Default;

    static Category create(const std::string& name, CategoryColor color);
};