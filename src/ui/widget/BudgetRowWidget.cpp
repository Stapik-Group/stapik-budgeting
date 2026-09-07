#include "BudgetRowWidget.hpp"

#include "../../core/util/AmountFormatter.hpp"
#include "../../core/util/CategoryColorUtils.hpp"
#include "../view/BudgetTableLayout.hpp"

#include <gtkmm/gestureclick.h>
#include <pangomm/layout.h>

BudgetRowWidget::BudgetRowWidget(const BudgetEntry& entry, const Category& category,
    const double plannedRemaining, const double actualRemaining) :
    Box(Gtk::Orientation::HORIZONTAL, BOX_SPACING)
{
    initLayout(entry, category, plannedRemaining, actualRemaining);
    initGesture();
}

void BudgetRowWidget::initLayout(const BudgetEntry& entry, const Category& category,
    const double plannedRemaining, const double actualRemaining)
{
    using namespace BudgetTableLayout;

    m_categoryLabel.set_text(category.name);
    m_categoryLabel.set_width_chars(CATEGORY_COLUMN_WIDTH_CHARS);
    m_categoryLabel.add_css_class(CategoryColorUtils::toCssClass(category.color));

    m_expenseLabel.set_text(entry.name);
    m_expenseLabel.set_width_chars(EXPENSE_COLUMN_WIDTH_CHARS);
    m_expenseLabel.set_halign(Gtk::Align::START);
    m_expenseLabel.set_ellipsize(Pango::EllipsizeMode::END);
    m_expenseLabel.add_css_class("budget-row-expense");

    m_plannedAmountLabel.set_text(AmountFormatter::format(entry.plannedAmount));
    m_plannedAmountLabel.set_width_chars(AMOUNT_COLUMN_WIDTH_CHARS);

    m_plannedRemainingLabel.set_text(AmountFormatter::format(plannedRemaining));
    m_plannedRemainingLabel.set_width_chars(REMAINING_COLUMN_WIDTH_CHARS);

    m_actualAmountLabel.set_text(AmountFormatter::format(entry.actualAmount));
    m_actualAmountLabel.set_width_chars(AMOUNT_COLUMN_WIDTH_CHARS);

    m_actualRemainingLabel.set_text(AmountFormatter::format(actualRemaining));
    m_actualRemainingLabel.set_width_chars(REMAINING_COLUMN_WIDTH_CHARS);

    m_moveUpButton.set_label("↑");
    m_moveUpButton.set_has_frame(false);
    m_moveUpButton.signal_clicked().connect([this] { m_signalMoveUpRequested.emit(); });

    m_moveDownButton.set_label("↓");
    m_moveDownButton.set_has_frame(false);
    m_moveDownButton.signal_clicked().connect([this] { m_signalMoveDownRequested.emit(); });

    m_deleteButton.set_label("✕");
    m_deleteButton.set_has_frame(false);
    m_deleteButton.add_css_class("budget-row-delete");
    m_deleteButton.signal_clicked().connect([this] { m_signalDeleteRequested.emit(); });

    append(m_categoryLabel);
    append(m_expenseLabel);
    append(m_plannedAmountLabel);
    append(m_plannedRemainingLabel);
    append(m_actualAmountLabel);
    append(m_actualRemainingLabel);
    append(m_moveUpButton);
    append(m_moveDownButton);
    append(m_deleteButton);
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