#include "MainWindow.hpp"

MainWindow::MainWindow() :
    m_placeholderLabel("Stapik Budgeting")
{
    init();
}

void MainWindow::init()
{
    set_title(WINDOW_TITLE);
    set_default_size(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    set_child(m_placeholderLabel);
}