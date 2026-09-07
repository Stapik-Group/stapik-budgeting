#include "BudgetHeader.hpp"

#include "BudgetTableLayout.hpp"

BudgetHeader::BudgetHeader() : Box(Gtk::Orientation::HORIZONTAL, BOX_SPACING)
{
    initLayout();
}

void BudgetHeader::initLayout()
{
    using namespace BudgetTableLayout;

    add_css_class("budget-header");

    m_categoryLabel.set_text("Kategoria");
    m_categoryLabel.set_width_chars(CATEGORY_COLUMN_WIDTH_CHARS);

    m_expenseLabel.set_text("Wydatek");
    m_expenseLabel.set_width_chars(EXPENSE_COLUMN_WIDTH_CHARS);
    m_expenseLabel.set_halign(Gtk::Align::START);

    m_plannedAmountLabel.set_text("Planowany: Kwota");
    m_plannedAmountLabel.set_width_chars(AMOUNT_COLUMN_WIDTH_CHARS);

    m_plannedRemainingLabel.set_text("Planowany: Pozostało");
    m_plannedRemainingLabel.set_width_chars(REMAINING_COLUMN_WIDTH_CHARS);

    m_actualAmountLabel.set_text("Wykonany: Kwota");
    m_actualAmountLabel.set_width_chars(AMOUNT_COLUMN_WIDTH_CHARS);

    m_actualRemainingLabel.set_text("Wykonany: Pozostało");
    m_actualRemainingLabel.set_width_chars(REMAINING_COLUMN_WIDTH_CHARS);

    m_actionsSpacer.set_width_chars(ACTIONS_COLUMN_WIDTH_CHARS);

    append(m_categoryLabel);
    append(m_expenseLabel);
    append(m_plannedAmountLabel);
    append(m_plannedRemainingLabel);
    append(m_actualAmountLabel);
    append(m_actualRemainingLabel);
    append(m_actionsSpacer);
}