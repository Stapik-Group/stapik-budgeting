#include "BudgetNavBar.hpp"

#include <array>

namespace
{
    constexpr std::array<const char*, 12> MONTH_NAMES = {
        "Styczen", "Luty", "Marzec", "Kwiecien", "Maj", "Czerwiec",
        "Lipiec", "Sierpien", "Wrzesien", "Pazdziernik", "Listopad", "Grudzien"
    };
}

BudgetNavBar::BudgetNavBar() : Box(Gtk::Orientation::HORIZONTAL, BOX_SPACING)
{
    initButtons();
    initLayout();
}

void BudgetNavBar::initButtons()
{
    m_btnPrevMonth.set_label("◀");
    m_btnNextMonth.set_label("▶");

    m_btnPrevMonth.signal_clicked().connect([this] { m_signalPrevMonth.emit(); });
    m_btnNextMonth.signal_clicked().connect([this] { m_signalNextMonth.emit(); });
}

void BudgetNavBar::initLayout()
{
    add_css_class("budget-navbar");
    set_halign(Gtk::Align::CENTER);
    set_margin(BOX_MARGIN);

    append(m_btnPrevMonth);
    append(m_currentLabel);
    append(m_btnNextMonth);

    m_currentLabel.set_width_chars(LABEL_WIDTH_CHARS);
    m_currentLabel.set_halign(Gtk::Align::CENTER);
}

void BudgetNavBar::updateDisplay(const int year, const unsigned month)
{
    m_currentLabel.set_text(formatYearMonth(year, month));
}

std::string BudgetNavBar::formatYearMonth(const int year, const unsigned month)
{
    if (month < 1 || month > 12)
        return {};

    return std::string(MONTH_NAMES[month - 1]) + " " + std::to_string(year);
}

sigc::signal<void()>& BudgetNavBar::signalPrevMonth() { return m_signalPrevMonth; }
sigc::signal<void()>& BudgetNavBar::signalNextMonth() { return m_signalNextMonth; }