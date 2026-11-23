#pragma once

#include "IndexedBudgetEntryCommand.hpp"
#include "../model/BudgetEntry.hpp"

class DeleteEntryCommand : public IndexedBudgetEntryCommand
{
public:
    DeleteEntryCommand(BudgetDocument& document, stapik::domain::YearMonth month, std::size_t entryIndex);

    void execute() override;
    void undo() override;
    [[nodiscard]] std::string description() const override;

private:
    BudgetEntry m_deletedEntry;
};
