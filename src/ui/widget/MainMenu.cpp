#include "MainMenu.hpp"

#include "stapik/locale/LocaleManager.hpp"

namespace
{
    StandardMenuOptions menuOptions(BudgetController& controller, const stapik::theme::ThemeRegistry& themes)
    {
        StandardMenuOptions options;
        options.themes = &themes;
        options.undoStack = &controller.undoStack();
        return options;
    }
}

MainMenu::MainMenu(Gtk::ApplicationWindow& window, BudgetController& controller, const stapik::theme::ThemeRegistry& themes) :
    m_currencySetting(controller),
    m_menu(window, menuOptions(controller, themes)),
    m_actionHandler(window, controller)
{
    m_actionHandler.registerActions();
    m_currencySetting.addTo(m_menu.settings());
    addBudgetItems();
}

Gtk::PopoverMenuBar& MainMenu::getMenuBar()
{
    return m_menu.menuBar();
}

void MainMenu::addBudgetItems()
{
    m_menu.addToMenu(StandardMenu::Target::File, [](Gio::Menu& menu)
    {
        const auto& loc = LocaleManager::instance();
        menu.append(loc.translate("menu.file.disconnect"), "win.disconnect");
    });

    m_menu.addToMenu(StandardMenu::Target::Edit, [](Gio::Menu& menu)
    {
        const auto& loc = LocaleManager::instance();
        const auto categoriesSection = Gio::Menu::create();
        categoriesSection->append(loc.translate("menu.edit.manageCategories"), "win.manageCategories");
        menu.append_section(categoriesSection);
    });
}
