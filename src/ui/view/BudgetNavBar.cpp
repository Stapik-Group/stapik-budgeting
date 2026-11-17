#include "BudgetNavBar.hpp"

#include "stapik/locale/LocaleManager.hpp"

BudgetNavBar::BudgetNavBar() : Box(Gtk::Orientation::HORIZONTAL, BOX_SPACING)
{
    initButtons();
    initLayout();

    LocaleManager::instance().signalLocaleChanged().connect([this] { updateDisplay(m_displayedMonth); });
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

void BudgetNavBar::updateDisplay(const stapik::domain::YearMonth& month)
{
    m_displayedMonth = month;
    m_currentLabel.set_text(month.label(LocaleManager::instance()));
}

sigc::signal<void()>& BudgetNavBar::signalPrevMonth() { return m_signalPrevMonth; }
sigc::signal<void()>& BudgetNavBar::signalNextMonth() { return m_signalNextMonth; }
