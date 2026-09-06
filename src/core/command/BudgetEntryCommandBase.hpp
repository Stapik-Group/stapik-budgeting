#pragma once

#include "BudgetCommand.hpp"
#include "../model/BudgetPeriod.hpp"

class BudgetEntryCommandBase : public BudgetCommand
{
protected:
    explicit BudgetEntryCommandBase(BudgetPeriod& period);
    BudgetPeriod& m_period;
};