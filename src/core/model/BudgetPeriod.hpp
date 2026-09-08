#pragma once

#include <cstddef>
#include <optional>
#include <vector>

#include "BudgetEntry.hpp"

class BudgetPeriod
{
public:
    BudgetPeriod(int year, unsigned month, double startingAmount);

    [[nodiscard]] int getYear() const { return m_year; }
    [[nodiscard]] unsigned getMonth() const { return m_month; }
    [[nodiscard]] double getStartingAmount() const { return m_startingAmount; }
    void setStartingAmount(const double startingAmount) { m_startingAmount = startingAmount; }

    [[nodiscard]] const std::vector<BudgetEntry>& getEntries() const { return m_entries; }
    void addEntry(BudgetEntry entry);
    void insertEntry(std::size_t index, BudgetEntry entry);
    void removeEntry(std::size_t index);
    void replaceEntry(std::size_t index, BudgetEntry entry);
    void moveEntry(std::size_t fromIndex, std::size_t toIndex);

    [[nodiscard]] std::vector<double> remainingPlanned() const;
    [[nodiscard]] std::vector<double> remainingActual() const;

private:
    int m_year;
    unsigned m_month;
    double m_startingAmount;
    std::vector<BudgetEntry> m_entries;

    [[nodiscard]] std::vector<double> remainingFor(std::optional<double> BudgetEntry::* amountMember) const;
};