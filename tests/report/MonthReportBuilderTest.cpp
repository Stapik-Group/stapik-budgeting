#include "core/report/MonthReportBuilder.hpp"

#include <gtest/gtest.h>

using namespace stapik::domain;

namespace
{
    const std::vector<std::string> COLUMNS = { "Category", "Expense", "Planned", "Planned: Remaining", "Actual", "Actual: Remaining" };
    const Currency PLN{ .code = "PLN", .symbol = "zł", .symbolBeforeAmount = false, .decimalPlaces = 2 };

    BudgetPeriod samplePeriod()
    {
        BudgetPeriod period(YearMonth(2026, 11), Money(100000, PLN));
        period.addEntry(BudgetEntry{ .name = "Salary", .categoryId = "work", .type = BudgetEntryType::Income, .plannedAmount = Money(500000, PLN), .actualAmount = Money(480000, PLN) });
        period.addEntry(BudgetEntry{ .name = "Rent", .categoryId = "home", .type = BudgetEntryType::Expense, .plannedAmount = Money(200050, PLN), .actualAmount = std::nullopt });
        return period;
    }

    MonthReportBuilder builder()
    {
        return MonthReportBuilder(
            [](const std::string& categoryId) { return Category{ .id = categoryId, .name = "cat-" + categoryId, .color = CategoryColor::Gray }; },
            PLN,
            "en");
    }
}

TEST(MonthReportBuilderTest, LinesFollowTheEntriesInOrder)
{
    const auto report = builder().build(samplePeriod(), "November 2026", COLUMNS, MonthReportNumbers::Plain);

    ASSERT_EQ(report.lines.size(), 2U);
    EXPECT_EQ(report.lines[0].expense, "Salary");
    EXPECT_EQ(report.lines[0].category, "cat-work");
    EXPECT_EQ(report.lines[1].expense, "Rent");
}

TEST(MonthReportBuilderTest, PlainNumbersCarryNoSymbolOrGrouping)
{
    const auto report = builder().build(samplePeriod(), "November 2026", COLUMNS, MonthReportNumbers::Plain);

    EXPECT_EQ(report.lines[0].planned, "5000.00");
    EXPECT_EQ(report.lines[0].plannedRemaining, "6000.00");
    EXPECT_EQ(report.lines[0].actual, "4800.00");
}

TEST(MonthReportBuilderTest, MissingActualAmountIsAnEmptyCell)
{
    const auto report = builder().build(samplePeriod(), "November 2026", COLUMNS, MonthReportNumbers::Plain);

    EXPECT_TRUE(report.lines[1].actual.empty());
    EXPECT_EQ(report.lines[1].actualRemaining, "5800.00");
}

TEST(MonthReportBuilderTest, ReportCarriesTitleCurrencyAndSixColumns)
{
    const auto report = builder().build(samplePeriod(), "November 2026", COLUMNS, MonthReportNumbers::Plain);

    EXPECT_EQ(report.title, "November 2026");
    EXPECT_EQ(report.currencyCode, "PLN");
    EXPECT_EQ(report.columnTitles.size(), 6U);
}

TEST(MonthReportBuilderTest, NumbersWithSymbolMentionTheCurrencySymbol)
{
    const auto report = builder().build(samplePeriod(), "November 2026", COLUMNS, MonthReportNumbers::WithSymbol);

    EXPECT_NE(report.lines[0].planned.find("zł"), std::string::npos);
}
