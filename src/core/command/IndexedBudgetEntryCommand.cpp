#include "IndexedBudgetEntryCommand.hpp"

IndexedBudgetEntryCommand::IndexedBudgetEntryCommand(BudgetDocument& document, const stapik::domain::YearMonth month, const std::size_t entryIndex) :
    BudgetEntryCommandBase(document, month),
    m_entryIndex(entryIndex)
{
}
