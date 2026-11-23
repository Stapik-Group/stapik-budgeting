#include "BudgetNavBar.hpp"

BudgetNavBar::BudgetNavBar(const stapik::domain::YearMonth& month) :
    Box(Gtk::Orientation::HORIZONTAL, BOX_SPACING),
    m_monthPicker(month)
{
    initButtons();
    initLayout();
}

void BudgetNavBar::initButtons()
{
    m_btnPrevMonth.set_label("◀");
    m_btnNextMonth.set_label("▶");

    m_btnPrevMonth.signal_clicked().connect([this] { m_monthPicker.setMonth(m_monthPicker.month().previous()); });
    m_btnNextMonth.signal_clicked().connect([this] { m_monthPicker.setMonth(m_monthPicker.month().next()); });
}

void BudgetNavBar::initLayout()
{
    add_css_class("budget-navbar");
    set_halign(Gtk::Align::CENTER);
    set_margin(BOX_MARGIN);

    m_monthPicker.set_size_request(PICKER_MIN_WIDTH, -1);

    append(m_btnPrevMonth);
    append(m_monthPicker);
    append(m_btnNextMonth);
}

const stapik::domain::YearMonth& BudgetNavBar::month() const
{
    return m_monthPicker.month();
}

sigc::signal<void()>& BudgetNavBar::signalMonthChanged()
{
    return m_monthPicker.signalMonthChanged();
}
