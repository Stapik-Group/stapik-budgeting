#include "DeleteEntryCommand.hpp"

#include "stapik/locale/LocaleManager.hpp"

DeleteEntryCommand::DeleteEntryCommand(BudgetDocument& document, const stapik::domain::YearMonth month, const std::size_t entryIndex) :
    IndexedBudgetEntryCommand(document, month, entryIndex),
    m_deletedEntry(period().getEntries().at(entryIndex))
{
}

void DeleteEntryCommand::execute()
{
    period().removeEntry(m_entryIndex);
}

void DeleteEntryCommand::undo()
{
    period().insertEntry(m_entryIndex, m_deletedEntry);
}

std::string DeleteEntryCommand::description() const
{
    return LocaleManager::instance().translate("command.entry.delete");
}
