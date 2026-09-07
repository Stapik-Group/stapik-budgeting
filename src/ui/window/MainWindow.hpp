#pragma once

#include <gtkmm/applicationwindow.h>
#include <gtkmm/label.h>

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

    BudgetView m_budgetView;

    void init();
};