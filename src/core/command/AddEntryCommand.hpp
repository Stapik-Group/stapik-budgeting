#pragma once

#include "BudgetEntryCommandBase.hpp"
#include "../model/BudgetEntry.hpp"

class AddEntryCommand : public BudgetEntryCommandBase
{
public:
    AddEntryCommand(BudgetPeriod& period, BudgetEntry entry);
    void execute() override;
    void undo() override;
private:
    BudgetEntry m_entry;
};