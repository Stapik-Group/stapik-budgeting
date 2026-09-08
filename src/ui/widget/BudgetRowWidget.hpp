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

    Widget& getPlannedGroup() { return m_plannedGroup; }
    Widget& getActualGroup() { return m_actualGroup; }
    Widget& getActionsBox() { return m_actionsBox; }
private:
    Gtk::Label m_categoryLabel;
    Gtk::Label m_expenseLabel;

    Gtk::Box m_plannedGroup;
    Gtk::Label m_plannedAmountLabel;
    Gtk::Label m_plannedRemainingLabel;

    Gtk::Box m_actualGroup;
    Gtk::Label m_actualAmountLabel;
    Gtk::Label m_actualRemainingLabel;

    Gtk::Box m_actionsBox;
    Gtk::Button m_moveUpButton;
    Gtk::Button m_moveDownButton;
    Gtk::Button m_editButton;
    Gtk::Button m_deleteButton;

    sigc::signal<void()> m_signalEditRequested;
    sigc::signal<void()> m_signalDeleteRequested;
    sigc::signal<void()> m_signalMoveUpRequested;
    sigc::signal<void()> m_signalMoveDownRequested;

    void initLayout(const BudgetEntry& entry, const Category& category, double plannedRemaining, double actualRemaining);
    void initGesture();
};
