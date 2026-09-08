#pragma once

#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <sigc++/signal.h>

class BudgetNavBar : public Gtk::Box
{
public:
    explicit BudgetNavBar();
    void updateDisplay(int year, unsigned month);

    sigc::signal<void()>& signalPrevMonth();
    sigc::signal<void()>& signalNextMonth();

private:
    static constexpr int BOX_SPACING = 4;
    static constexpr int BOX_MARGIN = 4;
    static constexpr int LABEL_WIDTH_CHARS = 20;

    Gtk::Button m_btnPrevMonth;
    Gtk::Button m_btnNextMonth;
    Gtk::Label m_currentLabel;

    int m_lastYear = 0;
    unsigned m_lastMonth = 1;

    sigc::signal<void()> m_signalPrevMonth;
    sigc::signal<void()> m_signalNextMonth;

    void initLayout();
    void initButtons();

    static std::string formatYearMonth(int year, unsigned month);
};