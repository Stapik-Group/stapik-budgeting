#pragma once

#include "BudgetDocumentCommand.hpp"

#include "stapik/domain/Category.hpp"

class AddCategoryCommand : public BudgetDocumentCommand
{
public:
    AddCategoryCommand(BudgetDocument& document, stapik::domain::Category category);

    void execute() override;
    void undo() override;
    [[nodiscard]] std::string description() const override;

private:
    stapik::domain::Category m_category;
};
