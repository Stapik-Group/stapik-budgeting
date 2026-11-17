#pragma once

#include "../model/BudgetDocument.hpp"

#include "stapik/command/ICommand.hpp"

class BudgetDocumentCommand : public stapik::command::ICommand
{
protected:
    explicit BudgetDocumentCommand(BudgetDocument& document);

    BudgetDocument& m_document;
};
