#pragma once

#include "BudgetDocumentCommand.hpp"

#include "stapik/domain/Category.hpp"

#include <cstddef>
#include <string>

class DeleteCategoryCommand : public BudgetDocumentCommand
{
public:
    DeleteCategoryCommand(BudgetDocument& document, const std::string& categoryId);

    void execute() override;
    void undo() override;
    [[nodiscard]] std::string description() const override;

private:
    stapik::domain::Category m_category;
    std::size_t m_index;
};
