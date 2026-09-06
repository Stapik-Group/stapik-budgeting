#pragma once

#include <string>
#include <vector>

#include "../model/CategoryColor.hpp"

class CategoryColorUtils
{
public:
    static std::string toString(CategoryColor color);
    static CategoryColor fromString(const std::string& value);
    static std::string toCssClass(CategoryColor color);
    static std::vector<CategoryColor> allColors();
};