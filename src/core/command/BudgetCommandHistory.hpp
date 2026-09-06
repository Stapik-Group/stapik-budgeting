#pragma once

#include "BudgetCommand.hpp"

#include <memory>
#include <vector>

class BudgetCommandHistory
{
public:
    void execute(std::unique_ptr<BudgetCommand> command);
    void undo();
    void redo();
    [[nodiscard]] bool canUndo() const;
    [[nodiscard]] bool canRedo() const;
private:
    std::vector<std::unique_ptr<BudgetCommand>> m_undoStack;
    std::vector<std::unique_ptr<BudgetCommand>> m_redoStack;
};