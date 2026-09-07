#pragma once

#include <gtkmm/box.h>
#include <gtkmm/button.h>

#include "BudgetGrid.hpp"
#include "BudgetHeader.hpp"
#include "BudgetNavBar.hpp"

class BudgetView : public Gtk::Box
{
public:
    explicit BudgetView();
    BudgetGrid& getBudgetGrid();

private:
    static constexpr int TOOLBAR_SPACING = 4;
    static constexpr int TOOLBAR_MARGIN = 4;

    int m_currentYear;
    unsigned m_currentMonth;

    BudgetNavBar m_navBar;
    Gtk::Box m_toolBar;
    Gtk::Button m_addEntryButton;
    Gtk::Button m_manageCategoriesButton;
    BudgetHeader m_header;
    BudgetGrid m_grid;

    void initLayout();
    void initToolBar();
    void initNavigation();

    void navigatePrevMonth();
    void navigateNextMonth();

    void refreshView();

    void showAddEntryDialog();
    void showEditEntryDialog(std::size_t index);
    void showManageCategoriesDialog();
};