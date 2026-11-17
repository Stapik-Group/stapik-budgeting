#pragma once

#include "BudgetEntryCommandBase.hpp"
#include "../model/BudgetEntry.hpp"

class AddEntryCommand : public BudgetEntryCommandBase
{
public:
    AddEntryCommand(BudgetDocument& document, stapik::domain::YearMonth month, BudgetEntry entry);

    void execute() override;
    void undo() override;
    [[nodiscard]] std::string description() const override;

private:
    BudgetEntry m_entry;
};
