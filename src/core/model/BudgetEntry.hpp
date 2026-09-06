#pragma once

#include <optional>
#include <string>

struct BudgetEntry
{
    std::string name;
    std::string categoryId;
    std::optional<double> plannedAmount;
    std::optional<double> actualAmount;
};