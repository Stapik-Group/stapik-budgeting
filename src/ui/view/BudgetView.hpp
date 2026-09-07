#pragma once

#include <gtkmm/box.h>

#include "BudgetGrid.hpp"
#include "BudgetHeader.hpp"
#include "BudgetNavBar.hpp"

class BudgetView : public Gtk::Box
{
public:
    explicit BudgetView();
    BudgetGrid& getBudgetGrid();

private:
    int m_currentYear;
    unsigned m_currentMonth;

    BudgetNavBar m_navBar;
    BudgetHeader m_header;
    BudgetGrid m_grid;

    void initLayout();
    void initNavigation();

    void navigatePrevMonth();
    void navigateNextMonth();

    void refreshView();
};