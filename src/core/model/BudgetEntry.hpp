#pragma once

#include "stapik/domain/Money.hpp"

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
    std::optional<stapik::domain::Money> plannedAmount;
    std::optional<stapik::domain::Money> actualAmount;
};
