#pragma once

#include "BudgetDocumentCommand.hpp"

#include "stapik/domain/Category.hpp"

#include <string>

class EditCategoryCommand : public BudgetDocumentCommand
{
public:
    EditCategoryCommand(BudgetDocument& document, const std::string& categoryId, stapik::domain::Category updated);

    void execute() override;
    void undo() override;
    [[nodiscard]] std::string description() const override;

private:
    std::string m_categoryId;
    stapik::domain::Category m_updated;
    stapik::domain::Category m_previous;
};
