#include "MainWindow.hpp"

MainWindow::MainWindow() :
    m_mainBox(Gtk::Orientation::VERTICAL, 0),
    m_mainMenu(*this, m_budgetView.getBudgetGrid())
{
    init();
    initLayout();
}

void MainWindow::init()
{
    set_title(WINDOW_TITLE);
    set_default_size(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    set_child(m_mainBox);
}

void MainWindow::initLayout()
{
    m_mainBox.append(m_mainMenu.getMenuBar());
    m_mainBox.append(m_budgetView);
}
