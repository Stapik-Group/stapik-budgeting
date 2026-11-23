#include "core/model/BudgetAmountsMigration.hpp"

#include <gtest/gtest.h>

namespace
{
    nlohmann::json legacyDocument()
    {
        return nlohmann::json::parse(R"({
            "currency": "PLN",
            "periods": [
                {
                    "startingAmount": 100.5,
                    "entries": [
                        { "name": "Rent", "plannedAmount": 12.25, "actualAmount": 3 },
                        { "name": "Unpaid", "plannedAmount": 7 }
                    ]
                }
            ]
        })");
    }
}

TEST(BudgetAmountsMigrationTest, ConvertsWholeAmountsToMinorUnits)
{
    auto document = legacyDocument();

    BudgetAmountsMigration::toMinorUnits(document);

    const auto& period = document.at("periods").at(0);
    EXPECT_EQ(period.at("startingAmountMinor").get<std::int64_t>(), 10050);
    EXPECT_EQ(period.at("entries").at(0).at("plannedAmountMinor").get<std::int64_t>(), 1225);
    EXPECT_EQ(period.at("entries").at(0).at("actualAmountMinor").get<std::int64_t>(), 300);
    EXPECT_EQ(period.at("entries").at(1).at("plannedAmountMinor").get<std::int64_t>(), 700);
}

TEST(BudgetAmountsMigrationTest, RemovesTheLegacyKeys)
{
    auto document = legacyDocument();

    BudgetAmountsMigration::toMinorUnits(document);

    const auto& period = document.at("periods").at(0);
    EXPECT_FALSE(period.contains("startingAmount"));
    EXPECT_FALSE(period.at("entries").at(0).contains("plannedAmount"));
    EXPECT_FALSE(period.at("entries").at(0).contains("actualAmount"));
}

TEST(BudgetAmountsMigrationTest, LeavesAnEntryWithoutActualAmountWithoutOne)
{
    auto document = legacyDocument();

    BudgetAmountsMigration::toMinorUnits(document);

    EXPECT_FALSE(document.at("periods").at(0).at("entries").at(1).contains("actualAmountMinor"));
}

TEST(BudgetAmountsMigrationTest, RunningTwiceChangesNothingMore)
{
    auto once = legacyDocument();
    BudgetAmountsMigration::toMinorUnits(once);

    auto twice = once;
    BudgetAmountsMigration::toMinorUnits(twice);

    EXPECT_EQ(once, twice);
}

TEST(BudgetAmountsMigrationTest, DocumentWithoutPeriodsIsLeftAlone)
{
    nlohmann::json document = { { "currency", "PLN" } };

    BudgetAmountsMigration::toMinorUnits(document);

    EXPECT_EQ(document, nlohmann::json({ { "currency", "PLN" } }));
}
