#pragma once

#include <gtkmm/box.h>
#include <gtkmm/label.h>

class BudgetHeader : public Gtk::Box
{
public:
    explicit BudgetHeader();

private:
    static constexpr int BOX_SPACING = 4;

    Gtk::Label m_categoryLabel;
    Gtk::Label m_expenseLabel;
    Gtk::Label m_plannedAmountLabel;
    Gtk::Label m_plannedRemainingLabel;
    Gtk::Label m_actualAmountLabel;
    Gtk::Label m_actualRemainingLabel;
    Gtk::Label m_actionsSpacer;

    void initLayout();
};