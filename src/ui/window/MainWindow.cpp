#include "MainWindow.hpp"

#include "stapik/app/version.hpp"

MainWindow::MainWindow() :
    m_mainBox(Gtk::Orientation::VERTICAL, 0),
    m_mainMenu(*this, m_budgetView.getBudgetGrid()),
    m_cloudNotifications(*this, m_budgetView.getBudgetGrid().cloud())
{
    init();
    initLayout();
    flushCloudOnClose();
}

void MainWindow::init()
{
    set_title(std::string{stapik::app::build::DISPLAY_NAME});
    set_default_size(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    set_child(m_mainBox);
}

void MainWindow::initLayout()
{
    m_mainBox.append(m_mainMenu.getMenuBar());
    m_mainBox.append(m_budgetView);
}

void MainWindow::flushCloudOnClose()
{
    signal_close_request().connect([this]
    {
        m_budgetView.getBudgetGrid().cloud().flushAndWait();
        return false;
    }, false);
}
