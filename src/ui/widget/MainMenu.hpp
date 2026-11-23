#pragma once

#include "../action/MenuActionHandler.hpp"
#include "../settings/BudgetCurrencySetting.hpp"
#include "../view/BudgetGrid.hpp"

#include "stapik/theme/ThemeRegistry.hpp"
#include "stapik/ui/menu/StandardMenu.hpp"

#include <gtkmm/applicationwindow.h>
#include <gtkmm/popovermenubar.h>

class MainMenu
{
public:
    MainMenu(Gtk::ApplicationWindow& window, BudgetGrid& budgetGrid, const stapik::theme::ThemeRegistry& themes);

    [[nodiscard]] Gtk::PopoverMenuBar& getMenuBar();

private:
    void addBudgetItems();

    BudgetCurrencySetting m_currencySetting;
    StandardMenu m_menu;
    MenuActionHandler m_actionHandler;
};
