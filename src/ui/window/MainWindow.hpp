#pragma once

#include <gtkmm/applicationwindow.h>
#include <gtkmm/box.h>

#include "../widget/MainMenu.hpp"
#include "../view/BudgetView.hpp"

class MainWindow : public Gtk::ApplicationWindow
{
public:
    explicit MainWindow();
    ~MainWindow() override = default;

private:
    static constexpr int DEFAULT_WIDTH = 1280;
    static constexpr int DEFAULT_HEIGHT = 800;
    static constexpr auto WINDOW_TITLE = "Stapik Budgeting";

    Gtk::Box m_mainBox;
    BudgetView m_budgetView;
    MainMenu m_mainMenu;

    void init();
    void initLayout();
    void initCloud();
};