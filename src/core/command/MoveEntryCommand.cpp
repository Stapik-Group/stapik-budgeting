#include "MoveEntryCommand.hpp"

#include "stapik/locale/LocaleManager.hpp"

MoveEntryCommand::MoveEntryCommand(BudgetDocument& document, const stapik::domain::YearMonth month, const std::size_t fromIndex, const std::size_t toIndex) :
    BudgetEntryCommandBase(document, month),
    m_fromIndex(fromIndex),
    m_toIndex(toIndex)
{
}

void MoveEntryCommand::execute()
{
    period().moveEntry(m_fromIndex, m_toIndex);
}

void MoveEntryCommand::undo()
{
    period().moveEntry(m_toIndex, m_fromIndex);
}

std::string MoveEntryCommand::description() const
{
    return LocaleManager::instance().translate("command.entry.move");
}
