#include "MainWindow.hpp"

#include "../../AppInfo.hpp"
#include "stapik/storage/CloudStorageConfigStorage.hpp"

MainWindow::MainWindow() :
    m_mainBox(Gtk::Orientation::VERTICAL, 0),
    m_mainMenu(*this, m_budgetView.getBudgetGrid())
{
    init();
    initLayout();
    initCloud();
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

void MainWindow::initCloud()
{
    const auto config = CloudStorageConfigStorage::load(APP_NAME);
    if (!config.has_value())
        return;

    m_budgetView.getBudgetGrid().setCloudClient(
        std::make_unique<CloudStorageClient>(config.value(), BUDGET_FILENAME));
}