#pragma once

#include <gtkmm/box.h>
#include <gtkmm/button.h>

#include "stapik/ui/widget/StatusIndicator.hpp"

#include "../../app/BudgetController.hpp"
#include "BudgetGrid.hpp"
#include "BudgetHeader.hpp"
#include "BudgetNavBar.hpp"

class BudgetView : public Gtk::Box
{
public:
    explicit BudgetView(BudgetController& controller);

private:
    static constexpr int BOTTOM_BAR_MARGIN = 10;

    BudgetController& m_controller;

    BudgetNavBar m_navBar;
    BudgetHeader m_header;
    BudgetGrid m_grid;
    Box m_bottomBar;
    StatusIndicator m_syncStatus;
    Gtk::Button m_addEntryButton;

    void initLayout();
    void initBottomBar();
    void initNavigation();
    void updateLabels();

    void refreshView();
    void refreshSyncStatus();

    void showAddEntryDialog();
    void showEditEntryDialog(std::size_t index);
};
