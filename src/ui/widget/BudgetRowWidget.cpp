#include "BudgetRowWidget.hpp"

#include "../../core/util/AmountFormatter.hpp"
#include "../../core/util/CategoryColorUtils.hpp"
#include "../view/BudgetTableLayout.hpp"

#include <gtkmm/gestureclick.h>
#include <pangomm/layout.h>

BudgetRowWidget::BudgetRowWidget(const BudgetEntry& entry, const Category& category,
    const double plannedRemaining, const double actualRemaining) :
    Box(Gtk::Orientation::HORIZONTAL, BudgetTableLayout::COLUMN_SPACING),
    m_plannedGroup(Gtk::Orientation::HORIZONTAL, BudgetTableLayout::GROUP_INNER_SPACING),
    m_actualGroup(Gtk::Orientation::HORIZONTAL, BudgetTableLayout::GROUP_INNER_SPACING),
    m_actionsBox(Gtk::Orientation::HORIZONTAL, BudgetTableLayout::ROW_ACTION_SPACING)
{
    initLayout(entry, category, plannedRemaining, actualRemaining);
    initGesture();
}

void BudgetRowWidget::initLayout(const BudgetEntry& entry, const Category& category,
    const double plannedRemaining, const double actualRemaining)
{
    using namespace BudgetTableLayout;

    add_css_class("budget-row");

    m_categoryLabel.set_text(category.name);
    m_categoryLabel.set_width_chars(CATEGORY_COLUMN_WIDTH_CHARS);
    m_categoryLabel.set_halign(Gtk::Align::START);
    m_categoryLabel.add_css_class(CategoryColorUtils::toCssClass(category.color));

    m_expenseLabel.set_text(entry.name);
    m_expenseLabel.set_hexpand(true);
    m_expenseLabel.set_halign(Gtk::Align::START);
    m_expenseLabel.set_ellipsize(Pango::EllipsizeMode::END);
    m_expenseLabel.add_css_class("budget-row-expense");

    m_plannedGroup.add_css_class("budget-group-planned");
    m_plannedAmountLabel.set_text(AmountFormatter::format(entry.plannedAmount));
    m_plannedAmountLabel.set_width_chars(AMOUNT_COLUMN_WIDTH_CHARS);
    m_plannedRemainingLabel.set_text(AmountFormatter::format(plannedRemaining));
    m_plannedRemainingLabel.set_width_chars(REMAINING_COLUMN_WIDTH_CHARS);
    m_plannedGroup.append(m_plannedAmountLabel);
    m_plannedGroup.append(m_plannedRemainingLabel);

    m_actualGroup.add_css_class("budget-group-actual");
    m_actualAmountLabel.set_text(AmountFormatter::format(entry.actualAmount));
    m_actualAmountLabel.set_width_chars(AMOUNT_COLUMN_WIDTH_CHARS);
    m_actualRemainingLabel.set_text(AmountFormatter::format(actualRemaining));
    m_actualRemainingLabel.set_width_chars(REMAINING_COLUMN_WIDTH_CHARS);
    m_actualGroup.append(m_actualAmountLabel);
    m_actualGroup.append(m_actualRemainingLabel);

    m_moveUpButton.set_label("↑");
    m_moveUpButton.add_css_class("budget-row-action");
    m_moveUpButton.signal_clicked().connect([this] { m_signalMoveUpRequested.emit(); });

    m_moveDownButton.set_label("↓");
    m_moveDownButton.add_css_class("budget-row-action");
    m_moveDownButton.signal_clicked().connect([this] { m_signalMoveDownRequested.emit(); });

    m_editButton.set_label("✎");
    m_editButton.add_css_class("budget-row-action");
    m_editButton.signal_clicked().connect([this] { m_signalEditRequested.emit(); });

    m_deleteButton.set_label("✕");
    m_deleteButton.add_css_class("budget-row-action");
    m_deleteButton.add_css_class("budget-row-delete");
    m_deleteButton.signal_clicked().connect([this] { m_signalDeleteRequested.emit(); });

    m_actionsBox.append(m_moveUpButton);
    m_actionsBox.append(m_moveDownButton);
    m_actionsBox.append(m_editButton);
    m_actionsBox.append(m_deleteButton);

    append(m_categoryLabel);
    append(m_expenseLabel);
    append(m_plannedGroup);
    append(m_actualGroup);
    append(m_actionsBox);
}

void BudgetRowWidget::initGesture()
{
    const auto gesture = Gtk::GestureClick::create();
    gesture->set_button(1);
    gesture->signal_released().connect(
        [this](const int nPress, double, double)
        {
            if (nPress == 1)
                m_signalEditRequested.emit();
        });

    m_expenseLabel.add_controller(gesture);
}

sigc::signal<void()>& BudgetRowWidget::signalEditRequested() { return m_signalEditRequested; }
sigc::signal<void()>& BudgetRowWidget::signalDeleteRequested() { return m_signalDeleteRequested; }
sigc::signal<void()>& BudgetRowWidget::signalMoveUpRequested() { return m_signalMoveUpRequested; }
sigc::signal<void()>& BudgetRowWidget::signalMoveDownRequested() { return m_signalMoveDownRequested; }
