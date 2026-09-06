#include "MoveEntryCommand.hpp"

MoveEntryCommand::MoveEntryCommand(BudgetPeriod& period, const std::size_t fromIndex, const std::size_t toIndex) :
    BudgetEntryCommandBase(period),
    m_fromIndex(fromIndex),
    m_toIndex(toIndex) {}

void MoveEntryCommand::execute()
{
    m_period.moveEntry(m_fromIndex, m_toIndex);
}

void MoveEntryCommand::undo()
{
    m_period.moveEntry(m_toIndex, m_fromIndex);
}