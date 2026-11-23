#include "MenuActionHandler.hpp"

#include "stapik/locale/LocaleManager.hpp"
#include "stapik/log/Log.hpp"
#include "stapik/ui/dialog/ConnectDialog.hpp"
#include "stapik/ui/dialog/DialogUtils.hpp"

#include "../dialog/CategoriesManagerDialog.hpp"

MenuActionHandler::MenuActionHandler(Gtk::ApplicationWindow& window, BudgetGrid& budgetGrid):
    m_window(window),
    m_budgetGrid(budgetGrid) {}

void MenuActionHandler::registerActions()
{
    m_window.add_action("manageCategories", sigc::mem_fun(*this, &MenuActionHandler::onActionManageCategories));
    m_window.add_action("connect", sigc::mem_fun(*this, &MenuActionHandler::onActionConnect));
    m_syncAction = m_window.add_action("sync", sigc::mem_fun(*this, &MenuActionHandler::onActionSync));
    m_disconnectAction = m_window.add_action("disconnect", sigc::mem_fun(*this, &MenuActionHandler::onActionDisconnect));

    m_budgetGrid.cloud().signalStatusChanged().connect([this](stapik::sync::SyncStatus) { refreshCloudActions(); });
    refreshCloudActions();
}

void MenuActionHandler::refreshCloudActions() const
{
    const auto connected = m_budgetGrid.cloud().isConnected();
    m_syncAction->set_enabled(connected);
    m_disconnectAction->set_enabled(connected);
}

void MenuActionHandler::onActionManageCategories() const
{
    auto* dialog = new CategoriesManagerDialog(m_window, m_budgetGrid);
    dialog->signal_hide().connect([dialog] { delete dialog; });
    dialog->show();
}

void MenuActionHandler::onActionConnect() const
{
    showConnectDialog(m_window, m_budgetGrid.cloud().config(), [this](const CloudStorageConfig& config)
    {
        applyCloudConfig(config);
    });
}

void MenuActionHandler::applyCloudConfig(const CloudStorageConfig& config) const
{
    const auto& loc = LocaleManager::instance();

    if (m_budgetGrid.connectToCloud(config))
    {
        stapik::log::info("[Cloud] Connected: {}", config.apiUrl);
        showMessageDialog(m_window, loc.translate("cloud.connected"), loc.translate("cloud.connected.secondary"));
        return;
    }

    stapik::log::warning("[Cloud] Cannot connect to {}", config.apiUrl);
    showMessageDialog(m_window, loc.translate("cloud.failed.header"), loc.translate("cloud.failed.secondary"));
}

void MenuActionHandler::onActionSync() const
{
    m_budgetGrid.syncWithCloud();
}

void MenuActionHandler::onActionDisconnect() const
{
    m_budgetGrid.cloud().disconnect();
}
