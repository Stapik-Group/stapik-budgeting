#include "MainWindow.hpp"

MainWindow::MainWindow()
{
    init();
}

void MainWindow::init()
{
    set_title(WINDOW_TITLE);
    set_default_size(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    set_child(m_budgetView);
}