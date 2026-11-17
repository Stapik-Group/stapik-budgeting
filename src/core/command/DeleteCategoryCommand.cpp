#include "DeleteCategoryCommand.hpp"

#include "stapik/locale/LocaleManager.hpp"

#include <algorithm>
#include <iterator>
#include <stdexcept>

DeleteCategoryCommand::DeleteCategoryCommand(BudgetDocument& document, const std::string& categoryId) :
    BudgetDocumentCommand(document)
{
    const auto& categories = document.categories();
    const auto position = std::ranges::find(categories, categoryId, &stapik::domain::Category::id);
    if (position == categories.end())
        throw std::invalid_argument("DeleteCategoryCommand: unknown category");

    m_category = *position;
    m_index = static_cast<std::size_t>(std::distance(categories.begin(), position));
}

void DeleteCategoryCommand::execute()
{
    m_document.deleteCategory(m_category.id);
}

void DeleteCategoryCommand::undo()
{
    m_document.insertCategory(m_index, m_category);
}

std::string DeleteCategoryCommand::description() const
{
    return LocaleManager::instance().translate("command.category.delete");
}
