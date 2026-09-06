#pragma once

#include "BudgetEntryCommandBase.hpp"

#include <cstddef>

class MoveEntryCommand : public BudgetEntryCommandBase
{
public:
    MoveEntryCommand(BudgetPeriod& period, std::size_t fromIndex, std::size_t toIndex);
    void execute() override;
    void undo() override;
private:
    std::size_t m_fromIndex;
    std::size_t m_toIndex;
};