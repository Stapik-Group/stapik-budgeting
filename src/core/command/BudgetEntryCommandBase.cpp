#include "BudgetEntryCommandBase.hpp"

BudgetEntryCommandBase::BudgetEntryCommandBase(BudgetDocument& document, const stapik::domain::YearMonth month) :
    BudgetDocumentCommand(document),
    m_month(month)
{
}

BudgetPeriod& BudgetEntryCommandBase::period() const
{
    return m_document.periodFor(m_month);
}
