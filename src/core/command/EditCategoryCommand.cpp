#include "EditCategoryCommand.hpp"

#include "stapik/locale/LocaleManager.hpp"

#include <stdexcept>
#include <utility>

EditCategoryCommand::EditCategoryCommand(BudgetDocument& document, const std::string& categoryId, stapik::domain::Category updated) :
    BudgetDocumentCommand(document),
    m_categoryId(categoryId),
    m_updated(std::move(updated))
{
    const auto* existing = document.findCategory(categoryId);
    if (existing == nullptr)
        throw std::invalid_argument("EditCategoryCommand: unknown category");

    m_previous = *existing;
}

void EditCategoryCommand::execute()
{
    m_document.editCategory(m_categoryId, m_updated);
}

void EditCategoryCommand::undo()
{
    m_document.editCategory(m_categoryId, m_previous);
}

std::string EditCategoryCommand::description() const
{
    return LocaleManager::instance().translate("command.category.edit");
}
