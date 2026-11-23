#pragma once

#include <nlohmann/json.hpp>

namespace BudgetAmountsMigration
{
    void toMinorUnits(nlohmann::json& document);
}
