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
    static constexpr int BOTTOM_BAR_MARGIN = 10;

    int m_currentYear;
    unsigned m_currentMonth;

    BudgetNavBar m_navBar;
    BudgetHeader m_header;
    BudgetGrid m_grid;
    Gtk::Box m_bottomBar;
    Gtk::Button m_addEntryButton;

    void initLayout();
    void initBottomBar();
    void initNavigation();
    void updateLabels();

    void navigatePrevMonth();
    void navigateNextMonth();

    void refreshView();

    void showAddEntryDialog();
    void showEditEntryDialog(std::size_t index);
};
