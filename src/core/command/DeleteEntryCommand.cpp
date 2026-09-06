#include "DeleteEntryCommand.hpp"

DeleteEntryCommand::DeleteEntryCommand(BudgetPeriod& period, const std::size_t entryIndex) :
    IndexedBudgetEntryCommand(period, entryIndex),
    m_deletedEntry(period.getEntries().at(entryIndex)) {}

void DeleteEntryCommand::execute()
{
    m_period.removeEntry(m_entryIndex);
}

void DeleteEntryCommand::undo()
{
    m_period.insertEntry(m_entryIndex, m_deletedEntry);
}