#include "core/report/MonthCsv.hpp"

#include <gtest/gtest.h>

namespace
{
    MonthReport sampleReport()
    {
        return MonthReport{
            .title = "November 2026",
            .currencyCode = "PLN",
            .columnTitles = { "Category", "Expense", "Planned", "Planned: Remaining", "Actual", "Actual: Remaining" },
            .lines = { { .category = "Home", .expense = "Rent, flat", .planned = "2000.50", .plannedRemaining = "4000.00", .actual = "", .actualRemaining = "4800.00" } }
        };
    }
}

TEST(MonthCsvTest, HeaderRowComesFirst)
{
    const auto writer = MonthCsv::write(sampleReport(), "en");

    EXPECT_NE(writer.content().find("Category,Expense,Planned,Planned: Remaining,Actual,Actual: Remaining"), std::string::npos);
}

TEST(MonthCsvTest, FieldsWithTheSeparatorAreQuoted)
{
    const auto writer = MonthCsv::write(sampleReport(), "en");

    EXPECT_NE(writer.content().find("Home,\"Rent, flat\",2000.50,4000.00,,4800.00"), std::string::npos);
}

TEST(MonthCsvTest, PolishUsesTheSemicolonSeparator)
{
    const auto writer = MonthCsv::write(sampleReport(), "pl");

    EXPECT_NE(writer.content().find("Category;Expense;Planned"), std::string::npos);
}

TEST(MonthCsvTest, RecordsEndWithCrLf)
{
    const auto writer = MonthCsv::write(sampleReport(), "en");

    EXPECT_NE(writer.content().find("\r\n"), std::string::npos);
}
