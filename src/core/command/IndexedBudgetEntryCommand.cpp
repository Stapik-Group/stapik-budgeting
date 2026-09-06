#include "IndexedBudgetEntryCommand.hpp"

IndexedBudgetEntryCommand::IndexedBudgetEntryCommand(BudgetPeriod& period, const std::size_t entryIndex) :
    BudgetEntryCommandBase(period),
    m_entryIndex(entryIndex) {}