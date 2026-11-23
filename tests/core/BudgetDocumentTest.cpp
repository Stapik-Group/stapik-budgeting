#include "core/model/BudgetDocument.hpp"

#include <gtest/gtest.h>

using namespace stapik::domain;

namespace
{
    const Currency PLN{ .code = "PLN", .symbol = "zł", .symbolBeforeAmount = false, .decimalPlaces = 2 };
    const Currency JPY{ .code = "JPY", .symbol = "¥", .symbolBeforeAmount = true, .decimalPlaces = 0 };
    const YearMonth NOVEMBER(2026, 11);

    BudgetDocument documentWithOneEntry()
    {
        BudgetDocument document(PLN);
        document.addCategory(Category{ .id = "bills", .name = "Bills", .color = CategoryColor::Blue });

        auto& period = document.periodFor(NOVEMBER);
        period.setStartingAmount(Money(100000, PLN));
        period.addEntry(BudgetEntry{
            .name = "Electricity",
            .categoryId = "bills",
            .type = BudgetEntryType::Expense,
            .plannedAmount = Money(25050, PLN),
            .actualAmount = std::nullopt
        });
        return document;
    }
}

TEST(BudgetDocumentTest, JsonRoundTripKeepsAmountsAndCategories)
{
    const auto restored = BudgetDocument::fromJson(documentWithOneEntry().toJson());

    ASSERT_NE(restored.findPeriod(NOVEMBER), nullptr);
    const auto& period = *restored.findPeriod(NOVEMBER);
    EXPECT_EQ(period.getStartingAmount().minorUnits(), 100000);
    ASSERT_EQ(period.getEntries().size(), 1U);
    EXPECT_EQ(period.getEntries()[0].name, "Electricity");
    EXPECT_EQ(period.getEntries()[0].plannedAmount->minorUnits(), 25050);
    EXPECT_FALSE(period.getEntries()[0].actualAmount.has_value());
    ASSERT_NE(restored.findCategory("bills"), nullptr);
    EXPECT_EQ(restored.findCategory("bills")->name, "Bills");
    EXPECT_EQ(restored.currency().code, "PLN");
}

TEST(BudgetDocumentTest, SerializedAmountsAreIntegerMinorUnits)
{
    const auto json = documentWithOneEntry().toJson();

    const auto& entry = json.at("periods").at(0).at("entries").at(0);
    EXPECT_TRUE(entry.at("plannedAmountMinor").is_number_integer());
    EXPECT_EQ(entry.at("plannedAmountMinor").get<std::int64_t>(), 25050);
    EXPECT_FALSE(entry.contains("plannedAmount"));
}

TEST(BudgetDocumentTest, LegacyPayloadWithDecimalAmountsStillLoads)
{
    auto json = documentWithOneEntry().toJson();
    auto& period = json.at("periods").at(0);
    period.erase("startingAmountMinor");
    period["startingAmount"] = 1000.0;
    auto& entry = period.at("entries").at(0);
    entry.erase("plannedAmountMinor");
    entry["plannedAmount"] = 250.5;

    const auto restored = BudgetDocument::fromJson(json);

    const auto& loaded = *restored.findPeriod(NOVEMBER);
    EXPECT_EQ(loaded.getStartingAmount().minorUnits(), 100000);
    EXPECT_EQ(loaded.getEntries()[0].plannedAmount->minorUnits(), 25050);
}

TEST(BudgetDocumentTest, ChangeCurrencyKeepsTheFigures)
{
    auto document = documentWithOneEntry();

    document.changeCurrency(JPY);

    EXPECT_EQ(document.currency().code, "JPY");
    EXPECT_EQ(document.findPeriod(NOVEMBER)->getStartingAmount().minorUnits(), 1000);
}

TEST(BudgetDocumentTest, DeletedCategoryCanBeInsertedBackAtItsPlace)
{
    auto document = documentWithOneEntry();
    document.addCategory(Category{ .id = "fun", .name = "Fun", .color = CategoryColor::Red });
    const auto bills = *document.findCategory("bills");

    document.deleteCategory("bills");
    document.insertCategory(0, bills);

    ASSERT_EQ(document.categories().size(), 2U);
    EXPECT_EQ(document.categories()[0].id, "bills");
}

TEST(BudgetDocumentTest, TouchMovesLastUpdateForward)
{
    BudgetDocument document(PLN);
    const auto before = document.lastUpdate();

    document.touch();

    EXPECT_GT(document.lastUpdate(), before);
}
