#pragma once

#include "BudgetDocumentCommand.hpp"

#include "stapik/domain/YearMonth.hpp"

class BudgetEntryCommandBase : public BudgetDocumentCommand
{
protected:
    BudgetEntryCommandBase(BudgetDocument& document, stapik::domain::YearMonth month);

    [[nodiscard]] BudgetPeriod& period() const;

private:
    stapik::domain::YearMonth m_month;
};
