#include "MainMenu.hpp"

#include "stapik/domain/CurrencyCatalog.hpp"
#include "stapik/locale/LocaleManager.hpp"
#include "stapik/theme/ThemeManager.hpp"

#include <array>
#include <format>
#include <string_view>

namespace
{
    constexpr std::array<std::string_view, 3> MENU_CURRENCY_CODES{ "PLN", "EUR", "USD" };
}

MainMenu::MainMenu(Gtk::ApplicationWindow &window, BudgetGrid &budgetGrid) : m_window(window),
    m_budgetGrid(budgetGrid),
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
    menuFile->append(loc.translate("menu.file.connect"), "win.connect");
    menuFile->append(loc.translate("menu.file.sync"), "win.sync");
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
    for (const auto code : MENU_CURRENCY_CODES)
    {
        const auto* currency = stapik::domain::CurrencyCatalog::instance().find(code);
        if (currency == nullptr)
            continue;

        menuCurrency->append(
            std::format("{} ({})", loc.translate(currency->nameKey()), currency->code),
            std::format("win.setCurrency::{}", currency->code)
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
    auto action = Gio::SimpleAction::create_radio_string("setLanguage", LocaleManager::instance().languageCode());

    action->signal_activate().connect([action](const Glib::VariantBase &parameter)
    {
        const auto value = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring> >(parameter).get();

        action->change_state(value);
        LocaleManager::instance().setLanguage(value.raw());
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

void MainMenu::initCurrencyAction()
{
    m_currencyAction = Gio::SimpleAction::create_radio_string("setCurrency", m_budgetGrid.currency().code);

    m_currencyAction->signal_activate().connect([this](const Glib::VariantBase& parameter)
    {
        const auto value = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(parameter).get();
        m_budgetGrid.changeCurrency(value.raw());
    });

    m_budgetGrid.signalCurrencyChanged().connect([this]
    {
        m_currencyAction->change_state(Glib::ustring(m_budgetGrid.currency().code));
    });

    m_window.add_action(m_currencyAction);
}
