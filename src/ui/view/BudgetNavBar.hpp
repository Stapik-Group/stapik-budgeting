#pragma once

#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <sigc++/signal.h>

#include "stapik/domain/YearMonth.hpp"
#include "stapik/ui/widget/MonthPicker.hpp"

class BudgetNavBar : public Gtk::Box
{
public:
    explicit BudgetNavBar(const stapik::domain::YearMonth& month);

    [[nodiscard]] const stapik::domain::YearMonth& month() const;
    sigc::signal<void()>& signalMonthChanged();

private:
    static constexpr int BOX_SPACING = 4;
    static constexpr int BOX_MARGIN = 4;
    static constexpr int PICKER_MIN_WIDTH = 170;

    Gtk::Button m_btnPrevMonth;
    Gtk::Button m_btnNextMonth;
    MonthPicker m_monthPicker;

    void initLayout();
    void initButtons();
};
