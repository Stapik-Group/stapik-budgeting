#include "BudgetHeader.hpp"

#include "BudgetTableLayout.hpp"

#include "stapik/locale/LocaleManager.hpp"

BudgetHeader::BudgetHeader() :
    Box(Gtk::Orientation::HORIZONTAL, BudgetTableLayout::COLUMN_SPACING),
    m_plannedGroup(Gtk::Orientation::VERTICAL, 2),
    m_actualGroup(Gtk::Orientation::VERTICAL, 2)
{
    initLayout();
    LocaleManager::instance().signalLocaleChanged().connect([this] { updateLabels(); });
}

void BudgetHeader::initLayout()
{
    using namespace BudgetTableLayout;

    add_css_class("budget-header");
    set_margin_bottom(HEADER_BOTTOM_MARGIN);

    m_categoryLabel.set_width_chars(CATEGORY_COLUMN_WIDTH_CHARS);
    m_categoryLabel.set_halign(Gtk::Align::START);

    m_expenseLabel.set_hexpand(true);
    m_expenseLabel.set_halign(Gtk::Align::START);

    m_plannedTitle.add_css_class("budget-group-title");
    m_actualTitle.add_css_class("budget-group-title");

    m_plannedGroup.add_css_class("budget-group-planned");
    m_actualGroup.add_css_class("budget-group-actual");

    auto* plannedRow = Gtk::make_managed<Box>(Gtk::Orientation::HORIZONTAL, GROUP_INNER_SPACING);
    m_plannedAmountLabel.set_width_chars(AMOUNT_COLUMN_WIDTH_CHARS);
    m_plannedRemainingLabel.set_width_chars(REMAINING_COLUMN_WIDTH_CHARS);
    plannedRow->append(m_plannedAmountLabel);
    plannedRow->append(m_plannedRemainingLabel);
    m_plannedGroup.append(m_plannedTitle);
    m_plannedGroup.append(*plannedRow);

    auto* actualRow = Gtk::make_managed<Box>(Gtk::Orientation::HORIZONTAL, GROUP_INNER_SPACING);
    m_actualAmountLabel.set_width_chars(AMOUNT_COLUMN_WIDTH_CHARS);
    m_actualRemainingLabel.set_width_chars(REMAINING_COLUMN_WIDTH_CHARS);
    actualRow->append(m_actualAmountLabel);
    actualRow->append(m_actualRemainingLabel);
    m_actualGroup.append(m_actualTitle);
    m_actualGroup.append(*actualRow);

    m_actionsSpacer.set_width_chars(ACTIONS_COLUMN_WIDTH_CHARS);

    updateLabels();

    append(m_categoryLabel);
    append(m_expenseLabel);
    append(m_plannedGroup);
    append(m_actualGroup);
    append(m_actionsSpacer);
}

void BudgetHeader::updateLabels()
{
    const auto& loc = LocaleManager::instance();

    m_categoryLabel.set_text(loc.translate("view.header.category"));
    m_expenseLabel.set_text(loc.translate("view.header.expense"));

    m_plannedTitle.set_text(loc.translate("view.header.planned"));
    m_plannedAmountLabel.set_text(loc.translate("view.header.amount"));
    m_plannedRemainingLabel.set_text(loc.translate("view.header.remaining"));

    m_actualTitle.set_text(loc.translate("view.header.actual"));
    m_actualAmountLabel.set_text(loc.translate("view.header.amount"));
    m_actualRemainingLabel.set_text(loc.translate("view.header.remaining"));
}