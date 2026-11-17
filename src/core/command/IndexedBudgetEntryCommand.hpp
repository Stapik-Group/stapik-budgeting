#pragma once

#include "BudgetEntryCommandBase.hpp"

#include <cstddef>

class IndexedBudgetEntryCommand : public BudgetEntryCommandBase
{
protected:
    IndexedBudgetEntryCommand(BudgetDocument& document, stapik::domain::YearMonth month, std::size_t entryIndex);

    std::size_t m_entryIndex;
};
