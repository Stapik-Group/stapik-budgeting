#pragma once

#include "../../app/BudgetController.hpp"

#include "BudgetHeader.hpp"

#include "stapik/ui/widget/EmptyState.hpp"

#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/sizegroup.h>
#include <gtkmm/stack.h>

class BudgetGrid : public Gtk::Box
{
public:
    explicit BudgetGrid(BudgetController& controller);

    void displayMonth(const stapik::domain::YearMonth& month);
    void bindHeader(BudgetHeader& header) const;

    sigc::signal<void()>& signalAddEntryRequested();
    sigc::signal<void(std::size_t)>& signalEditEntryRequested();

private:
    BudgetController& m_controller;

    Gtk::Stack m_stack;
    Gtk::ScrolledWindow m_scrolledWindow;
    EmptyState m_emptyState;
    Box m_rowsBox;

    Glib::RefPtr<Gtk::SizeGroup> m_plannedSizeGroup;
    Glib::RefPtr<Gtk::SizeGroup> m_actualSizeGroup;
    Glib::RefPtr<Gtk::SizeGroup> m_actionsSizeGroup;

    sigc::signal<void()> m_signalAddEntryRequested;
    sigc::signal<void(std::size_t)> m_signalEditEntryRequested;

    void initLayout();
    void populateRows();
    void appendRows(const BudgetPeriod& period);
};
