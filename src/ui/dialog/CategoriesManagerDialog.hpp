#pragma once

#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/dialog.h>
#include <gtkmm/listbox.h>
#include <gtkmm/scrolledwindow.h>

class BudgetGrid;

class CategoriesManagerDialog : public Gtk::Dialog
{
public:
    CategoriesManagerDialog(Gtk::Window& parent, BudgetGrid& grid);

private:
    static constexpr int CONTENT_SPACING = 8;
    static constexpr int CONTENT_MARGIN = 16;
    static constexpr int DEFAULT_WIDTH = 420;
    static constexpr int DEFAULT_HEIGHT = 360;

    BudgetGrid& m_grid;

    Gtk::Box m_contentBox;
    Gtk::ScrolledWindow m_scrolledWindow;
    Gtk::ListBox m_listBox;
    Gtk::Button m_addButton;

    void initLayout();
    void refreshList();
    void showAddCategoryDialog();
    void showEditCategoryDialog(const std::string& categoryId);
};