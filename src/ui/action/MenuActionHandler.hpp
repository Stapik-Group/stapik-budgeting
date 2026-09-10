#pragma once
#include <gtkmm/applicationwindow.h>

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

    void onActionQuit() const;
    void onActionUndo() const;
    void onActionRedo() const;
    void onActionManageCategories() const;
    void onActionConnect() const;
    void onActionSync() const;

    void handleConnectResult(const CloudStorageConfig& config) const;
    void applyCloudConfig(const CloudStorageConfig& config) const;
};