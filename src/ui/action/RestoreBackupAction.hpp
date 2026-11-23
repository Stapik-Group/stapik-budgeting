#pragma once

#include "../../app/BudgetController.hpp"

#include "stapik/document/BackupInfo.hpp"

#include <gtkmm/applicationwindow.h>

class RestoreBackupAction
{
public:
    RestoreBackupAction(Gtk::ApplicationWindow& window, BudgetController& controller);

    void registerActions();

private:
    Gtk::ApplicationWindow& m_window;
    BudgetController& m_controller;

    void onActionRestoreBackup() const;
    void restore(const stapik::document::BackupInfo& backup) const;
};
