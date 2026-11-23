#include "BudgetPeriod.hpp"

#include "RestateIn.hpp"

#include <stdexcept>
#include <utility>

namespace
{
    void restateAmount(std::optional<stapik::domain::Money>& amount, const stapik::domain::Currency& currency)
    {
        if (amount.has_value())
            amount = restateIn(*amount, currency);
    }
}

BudgetPeriod::BudgetPeriod(const stapik::domain::YearMonth month, stapik::domain::Money startingAmount) :
    m_month(month),
    m_startingAmount(std::move(startingAmount))
{
}

void BudgetPeriod::addEntry(BudgetEntry entry)
{
    m_entries.push_back(std::move(entry));
}

void BudgetPeriod::insertEntry(const std::size_t index, BudgetEntry entry)
{
    if (index > m_entries.size())
        throw std::out_of_range("BudgetPeriod::insertEntry: index out of range");
    
    m_entries.insert(m_entries.begin() + static_cast<std::ptrdiff_t>(index), std::move(entry));
}

void BudgetPeriod::removeEntry(const std::size_t index)
{
    if (index >= m_entries.size())
        throw std::out_of_range("BudgetPeriod::removeEntry: index out of range");

    m_entries.erase(m_entries.begin() + static_cast<std::ptrdiff_t>(index));
}

void BudgetPeriod::replaceEntry(const std::size_t index, BudgetEntry entry)
{
    if (index >= m_entries.size())
        throw std::out_of_range("BudgetPeriod::replaceEntry: index out of range");

    m_entries[index] = std::move(entry);
}

void BudgetPeriod::moveEntry(const std::size_t fromIndex, const std::size_t toIndex)
{
    if (fromIndex >= m_entries.size() || toIndex >= m_entries.size())
        throw std::out_of_range("BudgetPeriod::moveEntry: index out of range");

    if (fromIndex == toIndex)
        return;

    auto entry = std::move(m_entries[fromIndex]);
    m_entries.erase(m_entries.begin() + static_cast<std::ptrdiff_t>(fromIndex));
    m_entries.insert(m_entries.begin() + static_cast<std::ptrdiff_t>(toIndex), std::move(entry));
}

void BudgetPeriod::changeCurrency(const stapik::domain::Currency& currency)
{
    m_startingAmount = restateIn(m_startingAmount, currency);

    for (auto& entry : m_entries)
    {
        restateAmount(entry.plannedAmount, currency);
        restateAmount(entry.actualAmount, currency);
    }
}

std::vector<stapik::domain::Money> BudgetPeriod::remainingPlanned() const
{
    return remainingFor(&BudgetEntry::plannedAmount);
}

std::vector<stapik::domain::Money> BudgetPeriod::remainingActual() const
{
    return remainingFor(&BudgetEntry::actualAmount);
}

std::vector<stapik::domain::Money> BudgetPeriod::remainingFor(std::optional<stapik::domain::Money> BudgetEntry::* const amountMember) const
{
    std::vector<stapik::domain::Money> remaining;
    remaining.reserve(m_entries.size());

    auto runningTotal = m_startingAmount;
    for (const auto& entry : m_entries)
    {
        if (const auto& amount = entry.*amountMember; amount.has_value())
        {
            if (entry.type == BudgetEntryType::Income)
                runningTotal += *amount;
            else
                runningTotal -= *amount;
        }
        remaining.push_back(runningTotal);
    }
    return remaining;
}
