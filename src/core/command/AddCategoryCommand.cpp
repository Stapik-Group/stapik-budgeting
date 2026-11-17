#include "AddCategoryCommand.hpp"

#include "stapik/locale/LocaleManager.hpp"

#include <utility>

AddCategoryCommand::AddCategoryCommand(BudgetDocument& document, stapik::domain::Category category) :
    BudgetDocumentCommand(document),
    m_category(std::move(category))
{
}

void AddCategoryCommand::execute()
{
    m_document.addCategory(m_category);
}

void AddCategoryCommand::undo()
{
    m_document.deleteCategory(m_category.id);
}

std::string AddCategoryCommand::description() const
{
    return LocaleManager::instance().translate("command.category.add");
}
