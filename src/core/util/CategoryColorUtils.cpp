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
        using enum CategoryColor;
        case Red: return COLOR_RED;
        case Green: return COLOR_GREEN;
        case Blue: return COLOR_BLUE;
        case Yellow: return COLOR_YELLOW;
        case Purple: return COLOR_PURPLE;
        case Orange: return COLOR_ORANGE;
        case Brown: return COLOR_BROWN;
        case Pink: return COLOR_PINK;
        case Teal: return COLOR_TEAL;
        case Default:
        default: return COLOR_DEFAULT;
    }
}

CategoryColor CategoryColorUtils::fromString(const std::string& value)
{
    using enum CategoryColor;
    if (value == COLOR_RED) return Red;
    if (value == COLOR_GREEN) return Green;
    if (value == COLOR_BLUE) return Blue;
    if (value == COLOR_YELLOW) return Yellow;
    if (value == COLOR_PURPLE) return Purple;
    if (value == COLOR_ORANGE) return Orange;
    if (value == COLOR_BROWN) return Brown;
    if (value == COLOR_PINK) return Pink;
    if (value == COLOR_TEAL) return Teal;
    return Default;
}

std::string CategoryColorUtils::toCssClass(const CategoryColor color)
{
    return "category-color-" + toString(color);
}

std::vector<CategoryColor> CategoryColorUtils::allColors()
{
    using enum CategoryColor;
    return {
        Default,
        Red,
        Green,
        Blue,
        Yellow,
        Purple,
        Orange,
        Brown,
        Pink,
        Teal
    };
}