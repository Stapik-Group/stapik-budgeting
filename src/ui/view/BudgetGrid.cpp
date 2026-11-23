#include "BudgetGrid.hpp"
#include "BudgetHeader.hpp"

#include "../widget/BudgetRowWidget.hpp"

#include "stapik/locale/LocaleManager.hpp"

#include <gtkmm/widget.h>

namespace
{
    constexpr int ROWS_BOX_SPACING = 2;
    constexpr auto ROWS_PAGE = "rows";
    constexpr auto EMPTY_PAGE = "empty";
    constexpr auto EMPTY_STATE_ICON = "view-list-symbolic";
}

BudgetGrid::BudgetGrid(BudgetController& controller) :
    Box(Gtk::Orientation::VERTICAL, 0),
    m_controller(controller),
    m_emptyState(EMPTY_STATE_ICON, "view.empty.title"),
    m_rowsBox(Gtk::Orientation::VERTICAL, ROWS_BOX_SPACING)
{
    initLayout();
    populateRows();

    LocaleManager::instance().signalLocaleChanged().connect([this] { populateRows(); });
    m_controller.signalChanged().connect([this] { populateRows(); });
}

void BudgetGrid::initLayout()
{
    using enum Gtk::SizeGroup::Mode;
    m_plannedSizeGroup = Gtk::SizeGroup::create(HORIZONTAL);
    m_actualSizeGroup = Gtk::SizeGroup::create(HORIZONTAL);
    m_actionsSizeGroup = Gtk::SizeGroup::create(HORIZONTAL);

    set_hexpand(true);
    set_vexpand(true);

    m_scrolledWindow.set_hexpand(true);
    m_scrolledWindow.set_vexpand(true);
    m_scrolledWindow.set_child(m_rowsBox);

    m_emptyState.setDescriptionKey("view.empty.description");
    m_emptyState.setActionKey("view.toolbar.addEntry");
    m_emptyState.signalAction().connect([this] { m_signalAddEntryRequested.emit(); });

    m_stack.set_hexpand(true);
    m_stack.set_vexpand(true);
    m_stack.add(m_scrolledWindow, ROWS_PAGE);
    m_stack.add(m_emptyState, EMPTY_PAGE);

    append(m_stack);
}

void BudgetGrid::bindHeader(BudgetHeader& header) const
{
    m_plannedSizeGroup->add_widget(header.getPlannedGroup());
    m_actualSizeGroup->add_widget(header.getActualGroup());
    m_actionsSizeGroup->add_widget(header.getActionsSpacer());
}

void BudgetGrid::displayMonth(const stapik::domain::YearMonth& month)
{
    m_controller.setMonth(month);
    populateRows();
}

void BudgetGrid::populateRows()
{
    while (auto* child = m_rowsBox.get_first_child())
        m_rowsBox.remove(*child);

    const auto* period = m_controller.currentPeriod();
    const auto hasRows = period != nullptr && !period->getEntries().empty();

    if (hasRows)
        appendRows(*period);

    m_stack.set_visible_child(hasRows ? ROWS_PAGE : EMPTY_PAGE);
}

void BudgetGrid::appendRows(const BudgetPeriod& period)
{
    const auto plannedRemaining = period.remainingPlanned();
    const auto actualRemaining = period.remainingActual();
    const auto& entries = period.getEntries();

    for (std::size_t i = 0; i < entries.size(); ++i)
    {
        const auto& entry = entries[i];
        const auto& category = m_controller.categoryFor(entry.categoryId);

        auto* row = Gtk::make_managed<BudgetRowWidget>(entry, category, plannedRemaining[i], actualRemaining[i]);
        row->add_css_class(i % 2 == 0 ? "budget-row-even" : "budget-row-odd");

        m_plannedSizeGroup->add_widget(row->getPlannedGroup());
        m_actualSizeGroup->add_widget(row->getActualGroup());
        m_actionsSizeGroup->add_widget(row->getActionsBox());

        row->signalEditRequested().connect([this, i] { m_signalEditEntryRequested.emit(i); });
        row->signalDeleteRequested().connect([this, i] { m_controller.deleteEntryAt(i); });
        row->signalMoveUpRequested().connect([this, i] { m_controller.moveEntryUp(i); });
        row->signalMoveDownRequested().connect([this, i] { m_controller.moveEntryDown(i); });

        m_rowsBox.append(*row);
    }
}

sigc::signal<void()>& BudgetGrid::signalAddEntryRequested() { return m_signalAddEntryRequested; }
sigc::signal<void(std::size_t)>& BudgetGrid::signalEditEntryRequested() { return m_signalEditEntryRequested; }
