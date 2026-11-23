#include "AddEntryCommand.hpp"

#include "stapik/locale/LocaleManager.hpp"

#include <utility>

AddEntryCommand::AddEntryCommand(BudgetDocument& document, const stapik::domain::YearMonth month, BudgetEntry entry) :
    BudgetEntryCommandBase(document, month),
    m_entry(std::move(entry))
{
}

void AddEntryCommand::execute()
{
    period().addEntry(m_entry);
}

void AddEntryCommand::undo()
{
    auto& target = period();
    target.removeEntry(target.getEntries().size() - 1);
}

std::string AddEntryCommand::description() const
{
    return LocaleManager::instance().translate("command.entry.add");
}
