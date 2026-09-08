#pragma once

#include <gtkmm/box.h>
#include <gtkmm/label.h>

class BudgetHeader : public Gtk::Box
{
public:
    explicit BudgetHeader();

private:
    Gtk::Label m_categoryLabel;
    Gtk::Label m_expenseLabel;

    Box m_plannedGroup;
    Gtk::Label m_plannedTitle;
    Gtk::Label m_plannedAmountLabel;
    Gtk::Label m_plannedRemainingLabel;

    Box m_actualGroup;
    Gtk::Label m_actualTitle;
    Gtk::Label m_actualAmountLabel;
    Gtk::Label m_actualRemainingLabel;

    Gtk::Label m_actionsSpacer;

    void initLayout();
    void updateLabels();
};
