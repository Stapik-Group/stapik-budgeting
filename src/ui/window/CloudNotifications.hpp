#pragma once

#include "../../core/model/BudgetDocument.hpp"

#include "stapik/sync/CloudSession.hpp"

#include <gtkmm/window.h>
#include <sigc++/connection.h>

#include <optional>

class CloudNotifications
{
public:
    CloudNotifications(Gtk::Window& window, stapik::sync::CloudSession<BudgetDocument>& cloud);

private:
    void onApiKeyExpiring(BudgetDocument::TimePoint expiresAt) const;
    void onOutcome(const stapik::sync::SyncOutcome<BudgetDocument>& outcome);

    Gtk::Window& m_window;
    std::optional<stapik::sync::SyncState> m_lastReportedFailure;
    sigc::scoped_connection m_apiKeyConnection;
    sigc::scoped_connection m_outcomeConnection;
};
