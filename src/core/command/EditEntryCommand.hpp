#pragma once

#include "IndexedBudgetEntryCommand.hpp"
#include "../model/BudgetEntry.hpp"

class EditEntryCommand : public IndexedBudgetEntryCommand
{
public:
    EditEntryCommand(BudgetDocument& document, stapik::domain::YearMonth month, std::size_t entryIndex, BudgetEntry newEntry);

    void execute() override;
    void undo() override;
    [[nodiscard]] std::string description() const override;

private:
    BudgetEntry m_newEntry;
    BudgetEntry m_oldEntry;
};
