#include "core/command/AddEntryCommand.hpp"
#include "core/command/DeleteEntryCommand.hpp"
#include "core/command/MoveEntryCommand.hpp"

#include "stapik/command/UndoStack.hpp"

#include <gtest/gtest.h>

#include <memory>

using namespace stapik::domain;

namespace
{
    const Currency PLN{ .code = "PLN", .symbol = "zł", .symbolBeforeAmount = false, .decimalPlaces = 2 };
    const YearMonth NOVEMBER(2026, 11);

    BudgetEntry entryNamed(const std::string& name)
    {
        return BudgetEntry{ .name = name, .categoryId = "c1", .type = BudgetEntryType::Expense, .plannedAmount = Money(100, PLN), .actualAmount = std::nullopt };
    }

    class BudgetCommandsTest : public testing::Test
    {
    protected:
        BudgetDocument document{ PLN };
        stapik::command::UndoStack history;

        [[nodiscard]] const std::vector<BudgetEntry>& entries() const
        {
            return document.findPeriod(NOVEMBER)->getEntries();
        }
    };
}

TEST_F(BudgetCommandsTest, AddEntryAppendsAndUndoRemoves)
{
    history.execute(std::make_unique<AddEntryCommand>(document, NOVEMBER, entryNamed("First")));
    ASSERT_EQ(entries().size(), 1U);

    history.undo();
    EXPECT_TRUE(entries().empty());

    history.redo();
    ASSERT_EQ(entries().size(), 1U);
    EXPECT_EQ(entries()[0].name, "First");
}

TEST_F(BudgetCommandsTest, DeleteEntryUndoRestoresTheEntryAtItsPlace)
{
    history.execute(std::make_unique<AddEntryCommand>(document, NOVEMBER, entryNamed("A")));
    history.execute(std::make_unique<AddEntryCommand>(document, NOVEMBER, entryNamed("B")));
    history.execute(std::make_unique<AddEntryCommand>(document, NOVEMBER, entryNamed("C")));

    history.execute(std::make_unique<DeleteEntryCommand>(document, NOVEMBER, 1));
    ASSERT_EQ(entries().size(), 2U);
    EXPECT_EQ(entries()[1].name, "C");

    history.undo();
    ASSERT_EQ(entries().size(), 3U);
    EXPECT_EQ(entries()[1].name, "B");
}

TEST_F(BudgetCommandsTest, MoveEntryUndoRestoresTheOrder)
{
    history.execute(std::make_unique<AddEntryCommand>(document, NOVEMBER, entryNamed("A")));
    history.execute(std::make_unique<AddEntryCommand>(document, NOVEMBER, entryNamed("B")));

    history.execute(std::make_unique<MoveEntryCommand>(document, NOVEMBER, 0, 1));
    EXPECT_EQ(entries()[0].name, "B");

    history.undo();
    EXPECT_EQ(entries()[0].name, "A");
}

TEST_F(BudgetCommandsTest, UndoStackReportsWhatCanBeUndone)
{
    EXPECT_FALSE(history.canUndo());

    history.execute(std::make_unique<AddEntryCommand>(document, NOVEMBER, entryNamed("A")));

    EXPECT_TRUE(history.canUndo());
    EXPECT_FALSE(history.canRedo());
    EXPECT_EQ(history.undoDepth(), 1U);
}
