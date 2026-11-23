#pragma once

#include <gtkmm/applicationwindow.h>
#include <gtkmm/box.h>

#include "stapik/theme/ThemeRegistry.hpp"

#include "CloudNotifications.hpp"
#include "../widget/MainMenu.hpp"
#include "../view/BudgetView.hpp"

class MainWindow : public Gtk::ApplicationWindow
{
public:
    explicit MainWindow(const stapik::theme::ThemeRegistry& themes);
    ~MainWindow() override = default;

private:
    static constexpr int DEFAULT_WIDTH = 1280;
    static constexpr int DEFAULT_HEIGHT = 800;

    Gtk::Box m_mainBox;
    BudgetView m_budgetView;
    MainMenu m_mainMenu;
    CloudNotifications m_cloudNotifications;

    void init();
    void initLayout();
    void flushCloudOnClose();
};