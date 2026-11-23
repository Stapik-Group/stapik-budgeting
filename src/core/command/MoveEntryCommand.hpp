#pragma once

#include "BudgetEntryCommandBase.hpp"

#include <cstddef>

class MoveEntryCommand : public BudgetEntryCommandBase
{
public:
    MoveEntryCommand(BudgetDocument& document, stapik::domain::YearMonth month, std::size_t fromIndex, std::size_t toIndex);

    void execute() override;
    void undo() override;
    [[nodiscard]] std::string description() const override;

private:
    std::size_t m_fromIndex;
    std::size_t m_toIndex;
};
