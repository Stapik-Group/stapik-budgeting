#include "BudgetCommandHistory.hpp"

void BudgetCommandHistory::execute(std::unique_ptr<BudgetCommand> command)
{
    command->execute();
    m_undoStack.push_back(std::move(command));
    m_redoStack.clear();
}

void BudgetCommandHistory::undo()
{
    if (canUndo())
    {
        auto command = std::move(m_undoStack.back());
        m_undoStack.pop_back();
        command->undo();
        m_redoStack.push_back(std::move(command));
    }
}

void BudgetCommandHistory::redo()
{
    if (canRedo())
    {
        auto command = std::move(m_redoStack.back());
        m_redoStack.pop_back();
        command->execute();
        m_undoStack.push_back(std::move(command));
    }
}

bool BudgetCommandHistory::canUndo() const
{
    return !m_undoStack.empty();
}

bool BudgetCommandHistory::canRedo() const
{
    return !m_redoStack.empty();
}