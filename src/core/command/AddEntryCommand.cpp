#include "AddEntryCommand.hpp"

#include <utility>

AddEntryCommand::AddEntryCommand(BudgetPeriod& period, BudgetEntry entry) :
    BudgetEntryCommandBase(period),
    m_entry(std::move(entry)) {}

void AddEntryCommand::execute()
{
    m_period.addEntry(m_entry);
}

void AddEntryCommand::undo()
{
    m_period.removeEntry(m_period.getEntries().size() - 1);
}