#include "CategoriesManagerDialog.hpp"

#include "CategoryDialog.hpp"
#include "../view/BudgetGrid.hpp"
#include "../../core/util/CategoryColorUtils.hpp"

#include <algorithm>
#include <gtkmm/label.h>

CategoriesManagerDialog::CategoriesManagerDialog(Gtk::Window& parent, BudgetGrid& grid) :
    Dialog("Kategorie", parent, true),
    m_grid(grid),
    m_contentBox(Gtk::Orientation::VERTICAL, CONTENT_SPACING)
{
    initLayout();
    refreshList();
}

void CategoriesManagerDialog::initLayout()
{
    m_scrolledWindow.set_child(m_listBox);
    m_scrolledWindow.set_vexpand(true);

    m_addButton.set_label("Dodaj kategorię");
    m_addButton.signal_clicked().connect([this] { showAddCategoryDialog(); });

    m_contentBox.set_margin(CONTENT_MARGIN);
    m_contentBox.append(m_scrolledWindow);
    m_contentBox.append(m_addButton);

    get_content_area()->append(m_contentBox);

    add_button("Zamknij", Gtk::ResponseType::CLOSE);
    set_default_size(DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

void CategoriesManagerDialog::refreshList()
{
    while (auto* child = m_listBox.get_first_child())
        m_listBox.remove(*child);

    for (const auto& category : m_grid.getCategories())
    {
        auto* row = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, CONTENT_SPACING);

        auto* nameLabel = Gtk::make_managed<Gtk::Label>(category.name);
        nameLabel->set_halign(Gtk::Align::START);
        nameLabel->set_hexpand(true);
        nameLabel->add_css_class(CategoryColorUtils::toCssClass(category.color));

        const auto categoryId = category.id;

        auto* editButton = Gtk::make_managed<Gtk::Button>("Edytuj");
        editButton->signal_clicked().connect([this, categoryId] { showEditCategoryDialog(categoryId); });

        auto* deleteButton = Gtk::make_managed<Gtk::Button>("Usuń");
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