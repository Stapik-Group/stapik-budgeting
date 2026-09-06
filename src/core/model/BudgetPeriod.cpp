#include "BudgetPeriod.hpp"

#include <stdexcept>
#include <utility>

BudgetPeriod::BudgetPeriod(const int year, const unsigned month, const double startingAmount) :
    m_year(year),
    m_month(month),
    m_startingAmount(startingAmount)
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

std::vector<double> BudgetPeriod::remainingPlanned() const
{
    return remainingFor(&BudgetEntry::plannedAmount);
}

std::vector<double> BudgetPeriod::remainingActual() const
{
    return remainingFor(&BudgetEntry::actualAmount);
}

std::vector<double> BudgetPeriod::remainingFor(std::optional<double> BudgetEntry::* const amountMember) const
{
    std::vector<double> remaining;
    remaining.reserve(m_entries.size());

    double runningTotal = m_startingAmount;
    for (const auto& entry : m_entries)
    {
        if (const auto& amount = entry.*amountMember; amount.has_value())
            runningTotal -= amount.value();
        remaining.push_back(runningTotal);
    }
    return remaining;
}