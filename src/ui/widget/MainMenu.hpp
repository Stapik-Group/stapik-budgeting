#pragma once

#include <gtkmm/applicationwindow.h>
#include <gtkmm/popovermenubar.h>
#include <giomm/menu.h>

#include "../action/MenuActionHandler.hpp"
#include "../view/BudgetGrid.hpp"

class MainMenu
{
public:
    explicit MainMenu(Gtk::ApplicationWindow& window, BudgetGrid& budgetGrid);
    ~MainMenu() = default;
    Gtk::PopoverMenuBar& getMenuBar();

private:
    Gtk::ApplicationWindow& m_window;
    MenuActionHandler m_actionHandler;
    Glib::RefPtr<Gio::Menu> m_menuModel;
    Gtk::PopoverMenuBar m_menuBar;

    void buildModel();
    void initLanguageAction() const;
    void initThemeAction() const;
    void initCurrencyAction() const;
};
