#pragma once

class BudgetCommand
{
public:
    virtual ~BudgetCommand();
    virtual void execute() = 0;
    virtual void undo() = 0;
};