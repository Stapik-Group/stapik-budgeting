#include "MainMenu.hpp"

#include "../../core/currency/CurrencyManager.hpp"
#include "../../infrastructure/storage/CurrencyCatalog.hpp"
#include "stapik/locale/LocaleManager.hpp"
#include "stapik/theme/ThemeManager.hpp"

#include <format>

MainMenu::MainMenu(Gtk::ApplicationWindow &window, BudgetGrid &budgetGrid) : m_window(window),
    m_actionHandler(window, budgetGrid)
{
    m_actionHandler.registerActions();
    initLanguageAction();
    initThemeAction();
    initCurrencyAction();
    buildModel();
    LocaleManager::instance().signalLocaleChanged().connect([this] { buildModel(); });
}

Gtk::PopoverMenuBar &MainMenu::getMenuBar()
{
    return m_menuBar;
}

void MainMenu::buildModel()
{
    auto const &loc = LocaleManager::instance();

    m_menuModel = Gio::Menu::create();

    const auto menuFile = Gio::Menu::create();
    menuFile->append(loc.translate("menu.file.quit"), "win.quit");
    m_menuModel->append_submenu(loc.translate("menu.file"), menuFile);

    const auto menuEdit = Gio::Menu::create();
    menuEdit->append(loc.translate("menu.edit.undo"), "win.undo");
    menuEdit->append(loc.translate("menu.edit.redo"), "win.redo");
    const auto menuEditCategories = Gio::Menu::create();
    menuEditCategories->append(loc.translate("menu.edit.manageCategories"), "win.manageCategories");
    menuEdit->append_section(menuEditCategories);
    m_menuModel->append_submenu(loc.translate("menu.edit"), menuEdit);

    const auto menuLanguage = Gio::Menu::create();
    menuLanguage->append(loc.translate("menu.settings.language.pl"), "win.setLanguage::pl");
    menuLanguage->append(loc.translate("menu.settings.language.en"), "win.setLanguage::en");
    menuLanguage->append(loc.translate("menu.settings.language.de"), "win.setLanguage::de");

    const auto menuTheme = Gio::Menu::create();
    menuTheme->append(loc.translate("menu.settings.theme.classic"), "win.setTheme::classic");
    menuTheme->append(loc.translate("menu.settings.theme.classicPink"), "win.setTheme::classic-pink");
    menuTheme->append(loc.translate("menu.settings.theme.modern"), "win.setTheme::modern");

    const auto menuCurrency = Gio::Menu::create();
    for (const auto&[code, symbol] : CurrencyCatalog::instance().getCurrencies())
    {
        menuCurrency->append(
            std::format("{} ({})", loc.translate(std::format("currency.{}", code)), code),
            std::format("win.setCurrency::{}", code)
        );
    }

    const auto menuSettings = Gio::Menu::create();
    menuSettings->append_submenu(loc.translate("menu.settings.language"), menuLanguage);
    menuSettings->append_submenu(loc.translate("menu.settings.theme"), menuTheme);
    menuSettings->append_submenu(loc.translate("menu.settings.currency"), menuCurrency);
    m_menuModel->append_submenu(loc.translate("menu.settings"), menuSettings);

    m_menuBar.set_menu_model(m_menuModel);
}

void MainMenu::initLanguageAction() const
{
    const auto initialLocale = LocaleManager::instance().getLocale();
    std::string initialValue = "pl";
    if (initialLocale == Locale::EN) initialValue = "en";
    else if (initialLocale == Locale::DE) initialValue = "de";

    auto action = Gio::SimpleAction::create_radio_string("setLanguage", initialValue);

    action->signal_activate().connect([action](const Glib::VariantBase &parameter)
    {
        using enum Locale;
        const auto value = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring> >(parameter).get();

        action->change_state(value);

        if (value == "pl") LocaleManager::instance().setLocale(PL);
        else if (value == "en") LocaleManager::instance().setLocale(EN);
        else if (value == "de") LocaleManager::instance().setLocale(DE);
    });

    m_window.add_action(action);
}

void MainMenu::initThemeAction() const
{
    const auto currentTheme = ThemeManager::instance().getTheme();
    std::string initialValue = "classic";
    if (currentTheme == Theme::Modern) initialValue = "modern";
    else if (currentTheme == Theme::ClassicPink) initialValue = "classic-pink";

    auto action = Gio::SimpleAction::create_radio_string("setTheme", initialValue);
    action->signal_activate().connect([action](const Glib::VariantBase& parameter)
    {
        using enum Theme;
        const auto value = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(parameter).get();
        action->change_state(value);
        if (value == "modern") ThemeManager::instance().setTheme(Modern);
        else if (value == "classic-pink") ThemeManager::instance().setTheme(ClassicPink);
        else ThemeManager::instance().setTheme(Classic);
    });
    m_window.add_action(action);
}

void MainMenu::initCurrencyAction() const
{
    const auto currentCurrency = CurrencyManager::instance().getCurrency().code;

    auto action = Gio::SimpleAction::create_radio_string("setCurrency", currentCurrency);

    action->signal_activate().connect([action](const Glib::VariantBase& parameter)
    {
        const auto value = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(parameter).get();

        action->change_state(value);
        CurrencyManager::instance().setCurrency(value);
    });

    m_window.add_action(action);
}