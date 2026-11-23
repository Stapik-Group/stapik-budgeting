#pragma once

#include "../model/CategoryItem.hpp"

#include "stapik/ui/list/ListColumnView.hpp"
#include "stapik/ui/widget/EmptyState.hpp"
#include "stapik/ui/widget/SearchEntry.hpp"
#include "stapik/ui/widget/SectionHeader.hpp"

#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/dialog.h>
#include <gtkmm/stack.h>

class BudgetGrid;

class CategoriesManagerDialog : public Gtk::Dialog
{
public:
    CategoriesManagerDialog(Window& parent, BudgetGrid& grid);

private:
    static constexpr int CONTENT_SPACING = 8;
    static constexpr int CONTENT_MARGIN = 16;
    static constexpr int DEFAULT_WIDTH = 460;
    static constexpr int DEFAULT_HEIGHT = 420;

    BudgetGrid& m_grid;

    Gtk::Box m_contentBox;
    SectionHeader m_header;
    SearchEntry m_searchEntry;
    stapik::ui::ListColumnView<CategoryItem> m_list;
    EmptyState m_emptyState;
    Gtk::Stack m_stack;
    Gtk::Box m_buttonsBox;
    Gtk::Button m_addButton;
    Gtk::Button m_editButton;
    Gtk::Button m_deleteButton;

    void initLayout();
    void initList();
    void initButtons();
    void refreshList();
    void refreshButtons();
    void editSelected();
    void deleteSelected();
    void showAddCategoryDialog();
    void showEditCategoryDialog(const std::string& categoryId);
};
