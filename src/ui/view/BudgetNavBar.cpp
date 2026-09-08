#include "BudgetNavBar.hpp"

#include "stapik/locale/LocaleManager.hpp"

#include <array>

namespace
{
    constexpr std::array<const char*, 12> MONTH_KEYS = {
        "month.january", "month.february", "month.march", "month.april", "month.may", "month.june",
        "month.july", "month.august", "month.september", "month.october", "month.november", "month.december"
    };
}

BudgetNavBar::BudgetNavBar() : Box(Gtk::Orientation::HORIZONTAL, BOX_SPACING)
{
    initButtons();
    initLayout();

    LocaleManager::instance().signalLocaleChanged().connect([this] { updateDisplay(m_lastYear, m_lastMonth); });
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
    m_lastYear = year;
    m_lastMonth = month;
    m_currentLabel.set_text(formatYearMonth(year, month));
}

std::string BudgetNavBar::formatYearMonth(const int year, const unsigned month)
{
    if (month < 1 || month > 12)
        return {};

    return LocaleManager::instance().translate(MONTH_KEYS[month - 1]) + " " + std::to_string(year);
}

sigc::signal<void()>& BudgetNavBar::signalPrevMonth() { return m_signalPrevMonth; }
sigc::signal<void()>& BudgetNavBar::signalNextMonth() { return m_signalNextMonth; }
