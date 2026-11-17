#pragma once

#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

#include "BudgetEntry.hpp"

#include "stapik/domain/Currency.hpp"
#include "stapik/domain/Money.hpp"
#include "stapik/domain/YearMonth.hpp"

class BudgetPeriod
{
public:
    BudgetPeriod(stapik::domain::YearMonth month, stapik::domain::Money startingAmount);

    [[nodiscard]] const stapik::domain::YearMonth& getMonth() const { return m_month; }
    [[nodiscard]] const stapik::domain::Money& getStartingAmount() const { return m_startingAmount; }
    void setStartingAmount(stapik::domain::Money startingAmount) { m_startingAmount = std::move(startingAmount); }

    [[nodiscard]] const std::vector<BudgetEntry>& getEntries() const { return m_entries; }
    void addEntry(BudgetEntry entry);
    void insertEntry(std::size_t index, BudgetEntry entry);
    void removeEntry(std::size_t index);
    void replaceEntry(std::size_t index, BudgetEntry entry);
    void moveEntry(std::size_t fromIndex, std::size_t toIndex);

    void changeCurrency(const stapik::domain::Currency& currency);

    [[nodiscard]] std::vector<stapik::domain::Money> remainingPlanned() const;
    [[nodiscard]] std::vector<stapik::domain::Money> remainingActual() const;

private:
    stapik::domain::YearMonth m_month;
    stapik::domain::Money m_startingAmount;
    std::vector<BudgetEntry> m_entries;

    [[nodiscard]] std::vector<stapik::domain::Money> remainingFor(std::optional<stapik::domain::Money> BudgetEntry::* amountMember) const;
};