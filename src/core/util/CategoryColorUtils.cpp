#include "CategoryColorUtils.hpp"

namespace
{
    constexpr auto COLOR_DEFAULT = "default";
    constexpr auto COLOR_RED = "red";
    constexpr auto COLOR_GREEN = "green";
    constexpr auto COLOR_BLUE = "blue";
    constexpr auto COLOR_YELLOW = "yellow";
    constexpr auto COLOR_PURPLE = "purple";
    constexpr auto COLOR_ORANGE = "orange";
    constexpr auto COLOR_BROWN = "brown";
    constexpr auto COLOR_PINK = "pink";
    constexpr auto COLOR_TEAL = "teal";
}

std::string CategoryColorUtils::toString(const CategoryColor color)
{
    switch (color)
    {
        case CategoryColor::Red: return COLOR_RED;
        case CategoryColor::Green: return COLOR_GREEN;
        case CategoryColor::Blue: return COLOR_BLUE;
        case CategoryColor::Yellow: return COLOR_YELLOW;
        case CategoryColor::Purple: return COLOR_PURPLE;
        case CategoryColor::Orange: return COLOR_ORANGE;
        case CategoryColor::Brown: return COLOR_BROWN;
        case CategoryColor::Pink: return COLOR_PINK;
        case CategoryColor::Teal: return COLOR_TEAL;
        case CategoryColor::Default:
        default: return COLOR_DEFAULT;
    }
}

CategoryColor CategoryColorUtils::fromString(const std::string& value)
{
    if (value == COLOR_RED) return CategoryColor::Red;
    if (value == COLOR_GREEN) return CategoryColor::Green;
    if (value == COLOR_BLUE) return CategoryColor::Blue;
    if (value == COLOR_YELLOW) return CategoryColor::Yellow;
    if (value == COLOR_PURPLE) return CategoryColor::Purple;
    if (value == COLOR_ORANGE) return CategoryColor::Orange;
    if (value == COLOR_BROWN) return CategoryColor::Brown;
    if (value == COLOR_PINK) return CategoryColor::Pink;
    if (value == COLOR_TEAL) return CategoryColor::Teal;
    return CategoryColor::Default;
}

std::string CategoryColorUtils::toCssClass(const CategoryColor color)
{
    return "category-color-" + toString(color);
}

std::vector<CategoryColor> CategoryColorUtils::allColors()
{
    return {
        CategoryColor::Default,
        CategoryColor::Red,
        CategoryColor::Green,
        CategoryColor::Blue,
        CategoryColor::Yellow,
        CategoryColor::Purple,
        CategoryColor::Orange,
        CategoryColor::Brown,
        CategoryColor::Pink,
        CategoryColor::Teal
    };
}