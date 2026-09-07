#include "AmountFormatter.hpp"

#include <algorithm>
#include <format>

std::string AmountFormatter::format(const double amount)
{
    auto text = std::format("{:.2f}", amount);
    std::ranges::replace(text, '.', ',');
    return text + " zł";
}

std::string AmountFormatter::format(const std::optional<double>& amount)
{
    return amount.has_value() ? format(amount.value()) : std::string("—");
}