#include "BudgetView.hpp"

#include <chrono>

BudgetView::BudgetView() : Box(Gtk::Orientation::VERTICAL, 0)
{
    const auto today = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
    const std::chrono::year_month_day ymd{today};
    m_currentYear = static_cast<int>(ymd.year());
    m_currentMonth = static_cast<unsigned>(ymd.month());

    initLayout();
    initNavigation();
    refreshView();
}

void BudgetView::initLayout()
{
    set_hexpand(true);
    set_vexpand(true);

    append(m_navBar);
    append(m_header);
    append(m_grid);
}

void BudgetView::initNavigation()
{
    m_navBar.signalPrevMonth().connect(sigc::mem_fun(*this, &BudgetView::navigatePrevMonth));
    m_navBar.signalNextMonth().connect(sigc::mem_fun(*this, &BudgetView::navigateNextMonth));
}

void BudgetView::navigatePrevMonth()
{
    if (m_currentMonth == 1)
    {
        m_currentMonth = 12;
        --m_currentYear;
    }
    else
    {
        --m_currentMonth;
    }
    refreshView();
}

void BudgetView::navigateNextMonth()
{
    if (m_currentMonth == 12)
    {
        m_currentMonth = 1;
        ++m_currentYear;
    }
    else
    {
        ++m_currentMonth;
    }
    refreshView();
}

void BudgetView::refreshView()
{
    m_navBar.updateDisplay(m_currentYear, m_currentMonth);
    m_grid.displayMonth(m_currentYear, m_currentMonth);
}

BudgetGrid& BudgetView::getBudgetGrid()
{
    return m_grid;
}