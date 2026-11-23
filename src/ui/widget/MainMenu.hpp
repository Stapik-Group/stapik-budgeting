#pragma once

#include "../action/ArchiveActions.hpp"
#include "../action/MenuActionHandler.hpp"
#include "../action/MonthExportActions.hpp"
#include "../action/RestoreBackupAction.hpp"
#include "../settings/BudgetCurrencySetting.hpp"
#include "../../app/BudgetController.hpp"

#include "stapik/theme/ThemeRegistry.hpp"
#include "stapik/ui/menu/StandardMenu.hpp"

#include <gtkmm/applicationwindow.h>
#include <gtkmm/popovermenubar.h>

class MainMenu
{
public:
    MainMenu(Gtk::ApplicationWindow& window, BudgetController& controller, const stapik::theme::ThemeRegistry& themes);

    [[nodiscard]] Gtk::PopoverMenuBar& getMenuBar();

private:
    void addBudgetItems();

    BudgetCurrencySetting m_currencySetting;
    StandardMenu m_menu;
    MenuActionHandler m_actionHandler;
    RestoreBackupAction m_restoreBackup;
    ArchiveActions m_archive;
    MonthExportActions m_monthExport;
};
