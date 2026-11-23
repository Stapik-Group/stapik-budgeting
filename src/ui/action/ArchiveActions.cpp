#include "ArchiveActions.hpp"

#include "stapik/app/AppContext.hpp"
#include "stapik/app/version.hpp"
#include "stapik/locale/LocaleManager.hpp"
#include "stapik/settings/AppSettings.hpp"
#include "stapik/ui/dialog/ConfirmDialog.hpp"
#include "stapik/ui/dialog/DialogUtils.hpp"
#include "stapik/ui/dialog/FileDialogs.hpp"

#include <format>

namespace
{
    constexpr auto LAST_DIRECTORY_KEY = "archive";
}

ArchiveActions::ArchiveActions(Gtk::ApplicationWindow& window, BudgetController& controller) :
    m_window(window),
    m_controller(controller)
{
}

void ArchiveActions::registerActions()
{
    m_window.add_action("exportArchive", sigc::mem_fun(*this, &ArchiveActions::onActionExport));
    m_window.add_action("importArchive", sigc::mem_fun(*this, &ArchiveActions::onActionImport));
}

stapik::archive::AppArchiveContext ArchiveActions::context() const
{
    const auto& appName = stapik::app::AppContext::instance().info().internalName;

    return {
        .appName = appName,
        .appVersion = std::string{stapik::app::build::VERSION},
        .settings = &stapik::settings::appSettingsStore(appName),
        .assets = nullptr
    };
}

void ArchiveActions::onActionExport() const
{
    const auto& loc = LocaleManager::instance();

    FileDialogOptions options{
        .title = loc.translate("archive.export.title"),
        .initialName = std::format("{}-backup.zip", stapik::app::AppContext::instance().info().internalName),
        .lastDirectoryKey = LAST_DIRECTORY_KEY
    };

    showSaveFileDialog(m_window, options, [this](const std::filesystem::path& target) { exportTo(target); });
}

void ArchiveActions::onActionImport() const
{
    const auto& loc = LocaleManager::instance();

    FileDialogOptions options{
        .title = loc.translate("archive.import.title"),
        .lastDirectoryKey = LAST_DIRECTORY_KEY
    };

    showOpenFileDialog(m_window, options, [this](const std::filesystem::path& source)
    {
        const auto& texts = LocaleManager::instance();

        ConfirmDialogOptions confirm{
            .title = texts.translate("dialog.archive.import.title"),
            .message = texts.translate("dialog.archive.import.message"),
            .confirmLabel = texts.translate("dialog.archive.import.button"),
            .destructive = true
        };

        showConfirmDialog(m_window, confirm, [this, source] { importFrom(source); });
    });
}

void ArchiveActions::exportTo(const std::filesystem::path& target) const
{
    const auto& loc = LocaleManager::instance();

    const auto exported = stapik::archive::exportAppArchive(m_controller.documentFile(), m_controller.document(), context(), target);
    if (!exported)
    {
        showMessageDialog(m_window, loc.translate("archive.export.failed.header"), exported.error().message());
        return;
    }

    showMessageDialog(m_window, loc.translate("archive.export.done.header"), loc.translate("archive.export.done.secondary"));
}

void ArchiveActions::importFrom(const std::filesystem::path& source) const
{
    const auto& loc = LocaleManager::instance();

    auto imported = stapik::archive::importAppArchive(m_controller.documentFile(), source, context());
    if (!imported)
    {
        showMessageDialog(m_window, loc.translate("archive.import.failed.header"), imported.error().message());
        return;
    }

    const auto settingsRestored = imported->summary.settingsRestored;
    m_controller.restoreDocument(std::move(imported->document));

    showMessageDialog(m_window, loc.translate("archive.import.done.header"),
        loc.translate(settingsRestored ? "archive.import.restart.secondary" : "archive.import.done.secondary"));
}
