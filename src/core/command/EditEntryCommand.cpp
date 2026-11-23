#include "EditEntryCommand.hpp"

#include "stapik/locale/LocaleManager.hpp"

#include <utility>

EditEntryCommand::EditEntryCommand(BudgetDocument& document, const stapik::domain::YearMonth month, const std::size_t entryIndex, BudgetEntry newEntry) :
    IndexedBudgetEntryCommand(document, month, entryIndex),
    m_newEntry(std::move(newEntry)),
    m_oldEntry(period().getEntries().at(entryIndex))
{
}

void EditEntryCommand::execute()
{
    period().replaceEntry(m_entryIndex, m_newEntry);
}

void EditEntryCommand::undo()
{
    period().replaceEntry(m_entryIndex, m_oldEntry);
}

std::string EditEntryCommand::description() const
{
    return LocaleManager::instance().translate("command.entry.edit");
}
