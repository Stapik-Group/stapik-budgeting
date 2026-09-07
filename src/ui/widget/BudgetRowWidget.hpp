#pragma once

#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>

#include "../../core/model/BudgetEntry.hpp"
#include "../../core/model/Category.hpp"

class BudgetRowWidget : public Gtk::Box
{
public:
    BudgetRowWidget(const BudgetEntry& entry, const Category& category, double plannedRemaining, double actualRemaining);

    sigc::signal<void()>& signalEditRequested();
    sigc::signal<void()>& signalDeleteRequested();
    sigc::signal<void()>& signalMoveUpRequested();
    sigc::signal<void()>& signalMoveDownRequested();

private:
    static constexpr int BOX_SPACING = 4;

    Gtk::Label m_categoryLabel;
    Gtk::Label m_expenseLabel;
    Gtk::Label m_plannedAmountLabel;
    Gtk::Label m_plannedRemainingLabel;
    Gtk::Label m_actualAmountLabel;
    Gtk::Label m_actualRemainingLabel;
    Gtk::Button m_moveUpButton;
    Gtk::Button m_moveDownButton;
    Gtk::Button m_deleteButton;

    sigc::signal<void()> m_signalEditRequested;
    sigc::signal<void()> m_signalDeleteRequested;
    sigc::signal<void()> m_signalMoveUpRequested;
    sigc::signal<void()> m_signalMoveDownRequested;

    void initLayout(const BudgetEntry& entry, const Category& category, double plannedRemaining, double actualRemaining);
    void initGesture();
};