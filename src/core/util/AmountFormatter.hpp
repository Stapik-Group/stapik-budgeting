#pragma once

#include <optional>
#include <string>

class AmountFormatter
{
public:
    static std::string format(double amount);
    static std::string format(const std::optional<double>& amount);
};