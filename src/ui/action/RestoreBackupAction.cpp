#include "RestoreBackupAction.hpp"

#include "stapik/locale/LocaleManager.hpp"
#include "stapik/ui/dialog/DialogUtils.hpp"
#include "stapik/ui/dialog/RestoreBackupDialog.hpp"

RestoreBackupAction::RestoreBackupAction(Gtk::ApplicationWindow& window, BudgetController& controller) :
    m_window(window),
    m_controller(controller)
{
}

void RestoreBackupAction::registerActions()
{
    m_window.add_action("restoreBackup", sigc::mem_fun(*this, &RestoreBackupAction::onActionRestoreBackup));
}

void RestoreBackupAction::onActionRestoreBackup() const
{
    showRestoreBackupDialog(m_window, m_controller.documentFile().listBackups(),
        [this](const stapik::document::BackupInfo& backup) { restore(backup); });
}

void RestoreBackupAction::restore(const stapik::document::BackupInfo& backup) const
{
    auto loaded = m_controller.documentFile().loadBackup(backup);

    if (loaded.status != stapik::document::LoadStatus::Loaded || !loaded.document)
    {
        const auto& loc = LocaleManager::instance();
        showMessageDialog(m_window, loc.translate("backup.restoreFailed.header"), loc.translate("backup.restoreFailed.secondary"));
        return;
    }

    m_controller.restoreDocument(std::move(*loaded.document));
}
