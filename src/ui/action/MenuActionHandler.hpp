#pragma once
#include <gtkmm/applicationwindow.h>
#include <giomm/simpleaction.h>

#include "../../app/BudgetController.hpp"
#include "stapik/cloud/CloudStorageConfig.hpp"

class MenuActionHandler
{
public:
    explicit MenuActionHandler(Gtk::ApplicationWindow& window, BudgetController& controller);
    ~MenuActionHandler() = default;

    void registerActions();
private:
    Gtk::ApplicationWindow& m_window;
    BudgetController& m_controller;
    Glib::RefPtr<Gio::SimpleAction> m_syncAction;
    Glib::RefPtr<Gio::SimpleAction> m_disconnectAction;

    void refreshCloudActions() const;
    void onActionManageCategories() const;
    void onActionConnect() const;
    void onActionSync() const;
    void onActionDisconnect() const;

    void applyCloudConfig(const CloudStorageConfig& config) const;
};