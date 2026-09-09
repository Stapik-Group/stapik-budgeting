#pragma once

#include <optional>
#include <string>

enum class BudgetEntryType
{
    Expense,
    Income
};

struct BudgetEntry
{
    std::string name;
    std::string categoryId;
    BudgetEntryType type = BudgetEntryType::Expense;
    std::optional<double> plannedAmount;
    std::optional<double> actualAmount;
};