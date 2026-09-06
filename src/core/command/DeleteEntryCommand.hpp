#pragma once

#include "IndexedBudgetEntryCommand.hpp"
#include "../model/BudgetEntry.hpp"

class DeleteEntryCommand : public IndexedBudgetEntryCommand
{
public:
    DeleteEntryCommand(BudgetPeriod& period, std::size_t entryIndex);
    void execute() override;
    void undo() override;
private:
    BudgetEntry m_deletedEntry;
};