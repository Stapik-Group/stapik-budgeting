#include "CategoriesManagerDialog.hpp"

#include "CategoryDialog.hpp"
#include "../view/BudgetGrid.hpp"
#include "stapik/locale/LocaleManager.hpp"
#include "stapik/ui/widget/CategoryChip.hpp"

#include <algorithm>
#include <gtkmm/columnviewcolumn.h>
#include <gtkmm/listitem.h>
#include <gtkmm/signallistitemfactory.h>

namespace
{
    constexpr auto LIST_PAGE = "list";
    constexpr auto EMPTY_PAGE = "empty";
    constexpr auto EMPTY_STATE_ICON = "view-list-symbolic";

    Glib::RefPtr<Gtk::SignalListItemFactory> createChipFactory()
    {
        const auto factory = Gtk::SignalListItemFactory::create();

        factory->signal_setup().connect([](const Glib::RefPtr<Gtk::ListItem>& listItem)
        {
            listItem->set_child(*Gtk::make_managed<CategoryChip>(stapik::domain::Category{}));
        });

        factory->signal_bind().connect([](const Glib::RefPtr<Gtk::ListItem>& listItem)
        {
            const auto item = std::dynamic_pointer_cast<CategoryItem>(listItem->get_item());
            auto* chip = dynamic_cast<CategoryChip*>(listItem->get_child());
            if (item && chip)
                chip->setCategory(item->category());
        });

        return factory;
    }
}

CategoriesManagerDialog::CategoriesManagerDialog(Window& parent, BudgetGrid& grid) :
    Dialog(LocaleManager::instance().translate("dialog.categoriesManager.title"), parent, true),
    m_grid(grid),
    m_contentBox(Gtk::Orientation::VERTICAL, CONTENT_SPACING),
    m_header("dialog.categoriesManager.section"),
    m_emptyState(EMPTY_STATE_ICON, "dialog.categoriesManager.empty.title"),
    m_buttonsBox(Gtk::Orientation::HORIZONTAL, CONTENT_SPACING)
{
    initList();
    initButtons();
    initLayout();
    refreshList();

    signal_response().connect([this](int) { hide(); });
}

void CategoriesManagerDialog::initList()
{
    const auto& loc = LocaleManager::instance();

    auto column = Gtk::ColumnViewColumn::create(loc.translate("dialog.categoriesManager.column.name"), createChipFactory());
    column->set_expand(true);
    m_list.columnView().append_column(column);

    m_list.setSearchText([](const CategoryItem& item) { return item.category().name; });
    m_list.signalSelectionChanged().connect([this] { refreshButtons(); });
    m_list.signalActivated().connect([this](const Glib::RefPtr<CategoryItem>&) { editSelected(); });

    m_searchEntry.signalQueryChanged().connect([this] { m_list.setQuery(m_searchEntry.query()); });

    m_emptyState.setDescriptionKey("dialog.categoriesManager.empty.description");

    m_stack.set_vexpand(true);
    m_stack.add(m_list, LIST_PAGE);
    m_stack.add(m_emptyState, EMPTY_PAGE);
}

void CategoriesManagerDialog::initButtons()
{
    const auto& loc = LocaleManager::instance();

    m_addButton.set_label(loc.translate("dialog.categoriesManager.button.add"));
    m_addButton.add_css_class("suggested-action");
    m_addButton.signal_clicked().connect([this] { showAddCategoryDialog(); });

    m_editButton.set_label(loc.translate("dialog.categoriesManager.button.edit"));
    m_editButton.signal_clicked().connect([this] { editSelected(); });

    m_deleteButton.set_label(loc.translate("dialog.categoriesManager.button.delete"));
    m_deleteButton.signal_clicked().connect([this] { deleteSelected(); });

    m_buttonsBox.set_halign(Gtk::Align::END);
    m_buttonsBox.append(m_editButton);
    m_buttonsBox.append(m_deleteButton);
}

void CategoriesManagerDialog::initLayout()
{
    m_header.addAction(m_addButton);

    m_contentBox.set_margin(CONTENT_MARGIN);
    m_contentBox.append(m_header);
    m_contentBox.append(m_searchEntry);
    m_contentBox.append(m_stack);
    m_contentBox.append(m_buttonsBox);

    get_content_area()->append(m_contentBox);

    add_button(LocaleManager::instance().translate("dialog.categoriesManager.button.close"), Gtk::ResponseType::CLOSE);
    set_default_size(DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

void CategoriesManagerDialog::refreshList()
{
    std::vector<Glib::RefPtr<CategoryItem>> items;
    for (const auto& category : m_grid.getCategories())
        items.push_back(CategoryItem::create(category));

    m_list.setItems(items);
    m_stack.set_visible_child(items.empty() ? EMPTY_PAGE : LIST_PAGE);
    m_searchEntry.set_sensitive(!items.empty());
    refreshButtons();
}

void CategoriesManagerDialog::refreshButtons()
{
    const auto hasSelection = m_list.selectedItem() != nullptr;
    m_editButton.set_sensitive(hasSelection);
    m_deleteButton.set_sensitive(hasSelection);
}

void CategoriesManagerDialog::editSelected()
{
    if (const auto item = m_list.selectedItem())
        showEditCategoryDialog(item->category().id);
}

void CategoriesManagerDialog::deleteSelected()
{
    const auto item = m_list.selectedItem();
    if (item == nullptr)
        return;

    m_grid.deleteCategory(item->category().id);
    refreshList();
}

void CategoriesManagerDialog::showAddCategoryDialog()
{
    auto* dialog = new CategoryDialog(*this);

    dialog->signal_response().connect([this, dialog](const int responseId)
    {
        if (responseId == Gtk::ResponseType::OK)
        {
            if (auto result = dialog->getResult(); result.has_value())
            {
                m_grid.addCategory(std::move(result.value()));
                refreshList();
            }
        }
        dialog->hide();
    });

    dialog->signal_hide().connect([dialog] { delete dialog; });
    dialog->show();
}

void CategoriesManagerDialog::showEditCategoryDialog(const std::string& categoryId)
{
    const auto& categories = m_grid.getCategories();
    const auto it = std::ranges::find(categories, categoryId, &stapik::domain::Category::id);

    if (it == categories.end())
        return;

    auto* dialog = new CategoryDialog(*this, *it);

    dialog->signal_response().connect([this, dialog, categoryId](const int responseId)
    {
        if (responseId == Gtk::ResponseType::OK)
        {
            if (auto result = dialog->getResult(); result.has_value())
            {
                m_grid.editCategory(categoryId, std::move(result.value()));
                refreshList();
            }
        }
        dialog->hide();
    });

    dialog->signal_hide().connect([dialog] { delete dialog; });
    dialog->show();
}