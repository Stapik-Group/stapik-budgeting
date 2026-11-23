#include "CloudNotifications.hpp"

#include "stapik/locale/LocaleManager.hpp"
#include "stapik/sync/Timestamp.hpp"
#include "stapik/ui/dialog/DialogUtils.hpp"

#include <string>

namespace
{
    constexpr std::size_t ISO_DATE_LENGTH = 10;

    const char* failureKey(const stapik::sync::SyncState state)
    {
        using enum stapik::sync::SyncState;
        switch (state)
        {
            case AuthRejected: return "cloud.syncFailed.authRejected";
            case DecryptionFailed: return "cloud.syncFailed.decryptionFailed";
            case DocumentTooLarge: return "cloud.syncFailed.documentTooLarge";
            case RemoteUnreadable: return "cloud.syncFailed.remoteUnreadable";
            default: return nullptr;
        }
    }
}

CloudNotifications::CloudNotifications(Gtk::Window& window, stapik::sync::CloudSession<BudgetDocument>& cloud) :
    m_window(window),
    m_apiKeyConnection(cloud.signalApiKeyExpiring().connect([this](const BudgetDocument::TimePoint expiresAt) { onApiKeyExpiring(expiresAt); })),
    m_outcomeConnection(cloud.signalOutcome().connect([this](const stapik::sync::SyncOutcome<BudgetDocument>& outcome) { onOutcome(outcome); }))
{
}

void CloudNotifications::onApiKeyExpiring(const BudgetDocument::TimePoint expiresAt) const
{
    const auto& loc = LocaleManager::instance();
    const auto date = stapik::sync::toIso8601(expiresAt).substr(0, ISO_DATE_LENGTH);

    showMessageDialog(m_window, loc.translate("cloud.apiKeyExpiring.header"), loc.translate("cloud.apiKeyExpiring", { { "date", date } }));
}

void CloudNotifications::onOutcome(const stapik::sync::SyncOutcome<BudgetDocument>& outcome)
{
    const auto* key = failureKey(outcome.state);
    if (key == nullptr)
    {
        m_lastReportedFailure.reset();
        return;
    }

    if (m_lastReportedFailure == outcome.state)
        return;

    m_lastReportedFailure = outcome.state;

    const auto& loc = LocaleManager::instance();
    showMessageDialog(m_window, loc.translate("cloud.syncFailed.header"), loc.translate(key));
}
