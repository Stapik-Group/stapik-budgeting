#pragma once
#include <gtkmm/applicationwindow.h>
#include <giomm/simpleaction.h>

#include "../view/BudgetGrid.hpp"
#include "stapik/cloud/CloudStorageConfig.hpp"

class MenuActionHandler
{
public:
    explicit MenuActionHandler(Gtk::ApplicationWindow& window, BudgetGrid& budgetGrid);
    ~MenuActionHandler() = default;

    void registerActions();
private:
    Gtk::ApplicationWindow& m_window;
    BudgetGrid& m_budgetGrid;
    Glib::RefPtr<Gio::SimpleAction> m_syncAction;
    Glib::RefPtr<Gio::SimpleAction> m_disconnectAction;

    void refreshCloudActions() const;
    void onActionUndo() const;
    void onActionRedo() const;
    void onActionManageCategories() const;
    void onActionConnect() const;
    void onActionSync() const;
    void onActionDisconnect() const;

    void applyCloudConfig(const CloudStorageConfig& config) const;
};