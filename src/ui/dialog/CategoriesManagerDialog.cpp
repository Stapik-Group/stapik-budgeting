#include "CategoriesManagerDialog.hpp"

#include "CategoryDialog.hpp"
#include "../view/BudgetGrid.hpp"
#include "../../core/util/CategoryColorUtils.hpp"

#include "stapik/locale/LocaleManager.hpp"

#include <algorithm>
#include <gtkmm/label.h>

CategoriesManagerDialog::CategoriesManagerDialog(Window& parent, BudgetGrid& grid) :
    Dialog(LocaleManager::instance().translate("dialog.categoriesManager.title"), parent, true),
    m_grid(grid),
    m_contentBox(Gtk::Orientation::VERTICAL, CONTENT_SPACING)
{
    initLayout();
    refreshList();

    signal_response().connect([this](int) { hide(); });
}

void CategoriesManagerDialog::initLayout()
{
    const auto& loc = LocaleManager::instance();

    m_scrolledWindow.set_child(m_listBox);
    m_scrolledWindow.set_vexpand(true);

    m_addButton.set_label(loc.translate("dialog.categoriesManager.button.add"));
    m_addButton.signal_clicked().connect([this] { showAddCategoryDialog(); });

    m_contentBox.set_margin(CONTENT_MARGIN);
    m_contentBox.append(m_scrolledWindow);
    m_contentBox.append(m_addButton);

    get_content_area()->append(m_contentBox);

    add_button(loc.translate("dialog.categoriesManager.button.close"), Gtk::ResponseType::CLOSE);
    set_default_size(DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

void CategoriesManagerDialog::refreshList()
{
    const auto& loc = LocaleManager::instance();

    while (auto* child = m_listBox.get_first_child())
        m_listBox.remove(*child);

    for (const auto&[id, name, color] : m_grid.getCategories())
    {
        auto* row = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, CONTENT_SPACING);

        auto* nameLabel = Gtk::make_managed<Gtk::Label>(name);
        nameLabel->set_hexpand(true);
        nameLabel->set_halign(Gtk::Align::FILL);
        nameLabel->set_xalign(0.0f);
        nameLabel->add_css_class(CategoryColorUtils::toCssClass(color));

        const auto& categoryId = id;

        auto* editButton = Gtk::make_managed<Gtk::Button>(loc.translate("dialog.categoriesManager.button.edit"));
        editButton->signal_clicked().connect([this, categoryId] { showEditCategoryDialog(categoryId); });

        auto* deleteButton = Gtk::make_managed<Gtk::Button>(loc.translate("dialog.categoriesManager.button.delete"));
        deleteButton->signal_clicked().connect([this, categoryId]
        {
            m_grid.deleteCategory(categoryId);
            refreshList();
        });

        row->append(*nameLabel);
        row->append(*editButton);
        row->append(*deleteButton);

        m_listBox.append(*row);
    }
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
    const auto it = std::ranges::find_if(categories, [&categoryId](const Category& category)
    {
        return category.id == categoryId;
    });

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