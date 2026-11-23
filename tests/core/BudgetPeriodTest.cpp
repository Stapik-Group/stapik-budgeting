#include "core/model/BudgetPeriod.hpp"

#include <gtest/gtest.h>

using namespace stapik::domain;

namespace
{
    const Currency PLN{ .code = "PLN", .symbol = "zł", .symbolBeforeAmount = false, .decimalPlaces = 2 };
    const Currency JPY{ .code = "JPY", .symbol = "¥", .symbolBeforeAmount = true, .decimalPlaces = 0 };

    BudgetEntry entry(const std::string& name, const BudgetEntryType type, const std::int64_t planned, const std::int64_t actual)
    {
        return BudgetEntry{
            .name = name,
            .categoryId = "c1",
            .type = type,
            .plannedAmount = Money(planned, PLN),
            .actualAmount = Money(actual, PLN)
        };
    }

    BudgetPeriod periodWithEntries()
    {
        BudgetPeriod period(YearMonth(2026, 11), Money(100000, PLN));
        period.addEntry(entry("Salary", BudgetEntryType::Income, 500000, 480000));
        period.addEntry(entry("Rent", BudgetEntryType::Expense, 200000, 200000));
        return period;
    }
}

TEST(BudgetPeriodTest, RemainingPlannedIsRunningBalance)
{
    const auto remaining = periodWithEntries().remainingPlanned();

    ASSERT_EQ(remaining.size(), 2U);
    EXPECT_EQ(remaining[0].minorUnits(), 600000);
    EXPECT_EQ(remaining[1].minorUnits(), 400000);
}

TEST(BudgetPeriodTest, RemainingActualUsesActualAmounts)
{
    const auto remaining = periodWithEntries().remainingActual();

    ASSERT_EQ(remaining.size(), 2U);
    EXPECT_EQ(remaining[0].minorUnits(), 580000);
    EXPECT_EQ(remaining[1].minorUnits(), 380000);
}

TEST(BudgetPeriodTest, EntryWithoutAmountKeepsTheBalance)
{
    BudgetPeriod period(YearMonth(2026, 11), Money(1000, PLN));
    period.addEntry(BudgetEntry{ .name = "Planned only", .categoryId = "c1", .type = BudgetEntryType::Expense, .plannedAmount = Money(300, PLN), .actualAmount = std::nullopt });

    EXPECT_EQ(period.remainingPlanned()[0].minorUnits(), 700);
    EXPECT_EQ(period.remainingActual()[0].minorUnits(), 1000);
}

TEST(BudgetPeriodTest, MoveEntryReordersEntries)
{
    auto period = periodWithEntries();

    period.moveEntry(0, 1);

    EXPECT_EQ(period.getEntries()[0].name, "Rent");
    EXPECT_EQ(period.getEntries()[1].name, "Salary");
}

TEST(BudgetPeriodTest, OutOfRangeIndicesAreRejected)
{
    auto period = periodWithEntries();

    EXPECT_THROW(period.removeEntry(2), std::out_of_range);
    EXPECT_THROW(period.insertEntry(3, entry("x", BudgetEntryType::Expense, 1, 1)), std::out_of_range);
    EXPECT_THROW(period.moveEntry(0, 2), std::out_of_range);
}

TEST(BudgetPeriodTest, ChangeCurrencyKeepsTheFiguresAndAdaptsDecimalPlaces)
{
    auto period = periodWithEntries();

    period.changeCurrency(JPY);

    EXPECT_EQ(period.getStartingAmount().minorUnits(), 1000);
    EXPECT_EQ(period.getStartingAmount().currency().code, "JPY");
    EXPECT_EQ(period.getEntries()[0].plannedAmount->minorUnits(), 5000);
}
