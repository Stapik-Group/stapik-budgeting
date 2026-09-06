#pragma once

#include "IndexedBudgetEntryCommand.hpp"
#include "../model/BudgetEntry.hpp"

class EditEntryCommand : public IndexedBudgetEntryCommand
{
public:
    EditEntryCommand(BudgetPeriod& period, std::size_t entryIndex, BudgetEntry newEntry);
    void execute() override;
    void undo() override;
private:
    BudgetEntry m_newEntry;
    BudgetEntry m_oldEntry;
};