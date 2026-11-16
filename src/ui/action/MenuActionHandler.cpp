#include "MenuActionHandler.hpp"

#include "../../AppInfo.hpp"

#include "stapik/cloud/CloudStorageException.hpp"
#include "stapik/locale/LocaleManager.hpp"
#include "stapik/log/Log.hpp"
#include "stapik/storage/CloudStorageConfigStorage.hpp"
#include "stapik/ui/dialog/ConnectDialog.hpp"
#include "stapik/ui/dialog/DialogUtils.hpp"

#include "../dialog/CategoriesManagerDialog.hpp"

#include <tuple>

MenuActionHandler::MenuActionHandler(Gtk::ApplicationWindow& window, BudgetGrid& budgetGrid):
    m_window(window),
    m_budgetGrid(budgetGrid) {}

void MenuActionHandler::registerActions()
{
    m_window.add_action("undo", sigc::mem_fun(*this, &MenuActionHandler::onActionUndo));
    m_window.add_action("redo", sigc::mem_fun(*this, &MenuActionHandler::onActionRedo));
    m_window.add_action("manageCategories", sigc::mem_fun(*this, &MenuActionHandler::onActionManageCategories));
    m_window.add_action("connect", sigc::mem_fun(*this, &MenuActionHandler::onActionConnect));
    m_window.add_action("sync", sigc::mem_fun(*this, &MenuActionHandler::onActionSync));
}

void MenuActionHandler::onActionUndo() const
{
    m_budgetGrid.undo();
}

void MenuActionHandler::onActionRedo() const
{
    m_budgetGrid.redo();
}

void MenuActionHandler::onActionManageCategories() const
{
    auto* dialog = new CategoriesManagerDialog(m_window, m_budgetGrid);
    dialog->signal_hide().connect([dialog] { delete dialog; });
    dialog->show();
}

void MenuActionHandler::onActionConnect() const
{
    auto* dialog = new ConnectDialog(m_window);

    if (const auto config = CloudStorageConfigStorage::load(APP_NAME); config.has_value())
        dialog->prefillConfig(config.value());

    dialog->signal_response().connect([this, dialog](const int responseId)
    {
        if (responseId == Gtk::ResponseType::OK)
        {
            if (const auto result = dialog->getResult(); result.has_value())
                handleConnectResult(result.value());
        }
        dialog->hide();
    });

    dialog->signal_hide().connect([dialog] { delete dialog; });
    dialog->show();
}

void MenuActionHandler::handleConnectResult(const CloudStorageConfig& config) const
{
    CloudStorageConfigStorage::save(config, APP_NAME);
    applyCloudConfig(config);
}

void MenuActionHandler::applyCloudConfig(const CloudStorageConfig& config) const
{
    const auto& loc = LocaleManager::instance();

    try
    {
        auto client = std::make_unique<CloudStorageClient>(config, BUDGET_FILENAME);
        std::ignore = client->loadDocument();

        m_budgetGrid.setCloudClient(std::move(client));

        stapik::log::info("[Cloud] Connected: {}", config.apiUrl);
        showMessageDialog(m_window, loc.translate("cloud.connected"), loc.translate("cloud.connected.secondary"));
    }
    catch (const CloudStorageException& e)
    {
        stapik::log::warning("[Cloud] Cloud connection error: {}", e.what());
        showMessageDialog(m_window, loc.translate("cloud.failed.header"), e.what());
    }
}

void MenuActionHandler::onActionSync() const
{
    m_budgetGrid.retrySync();
}