#include "EditEntryCommand.hpp"

#include <utility>

EditEntryCommand::EditEntryCommand(BudgetPeriod& period, const std::size_t entryIndex, BudgetEntry newEntry) :
    IndexedBudgetEntryCommand(period, entryIndex),
    m_newEntry(std::move(newEntry)),
    m_oldEntry(period.getEntries().at(entryIndex)) {}

void EditEntryCommand::execute()
{
    m_period.replaceEntry(m_entryIndex, m_newEntry);
}

void EditEntryCommand::undo()
{
    m_period.replaceEntry(m_entryIndex, m_oldEntry);
}