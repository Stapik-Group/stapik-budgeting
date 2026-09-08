#pragma once
#include <gtkmm/applicationwindow.h>

#include "../view/BudgetGrid.hpp"

class MenuActionHandler
{
public:
    explicit MenuActionHandler(Gtk::ApplicationWindow& window, BudgetGrid& budgetGrid);
    ~MenuActionHandler() = default;

    void registerActions();
private:
    Gtk::ApplicationWindow& m_window;
    BudgetGrid& m_budgetGrid;

    void onActionQuit() const;
    void onActionUndo() const;
    void onActionRedo() const;
    void onActionManageCategories() const;
};