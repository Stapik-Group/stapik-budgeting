#include "BudgetEntryDialog.hpp"

#include "stapik/locale/LocaleManager.hpp"

BudgetEntryDialog::BudgetEntryDialog(Window& parent, const std::vector<stapik::domain::Category>& categories,
    const stapik::domain::Currency& currency) :
    Dialog(LocaleManager::instance().translate("dialog.entry.new.title"), parent, true),
    m_categories(categories),
    m_contentBox(Gtk::Orientation::VERTICAL, CONTENT_SPACING),
    m_plannedAmountEntry(currency),
    m_actualAmountEntry(currency)
{
    initLayout();
    populateCategoryCombo(m_categories.empty() ? std::string{} : m_categories.front().id);
    refreshAcceptable();
}

BudgetEntryDialog::BudgetEntryDialog(Window& parent, const std::vector<stapik::domain::Category>& categories,
    const stapik::domain::Currency& currency, const BudgetEntry& existing) :
    Dialog(LocaleManager::instance().translate("dialog.entry.edit.title"), parent, true),
    m_categories(categories),
    m_contentBox(Gtk::Orientation::VERTICAL, CONTENT_SPACING),
    m_plannedAmountEntry(currency),
    m_actualAmountEntry(currency)
{
    initLayout();
    populateCategoryCombo(existing.categoryId);
    selectType(existing.type);

    m_nameEntry.set_text(existing.name);

    if (existing.plannedAmount.has_value())
        m_plannedAmountEntry.setAmount(existing.plannedAmount->minorUnits());
    if (existing.actualAmount.has_value())
        m_actualAmountEntry.setAmount(existing.actualAmount->minorUnits());

    refreshAcceptable();
}

void BudgetEntryDialog::initLayout()
{
    const auto& loc = LocaleManager::instance();

    m_typeLabel.set_text(loc.translate("dialog.entry.type.label"));
    m_typeLabel.set_halign(Gtk::Align::START);

    m_typeBox.add_css_class("linked");
    m_expenseTypeButton.set_label(loc.translate("dialog.entry.type.expense"));
    m_incomeTypeButton.set_label(loc.translate("dialog.entry.type.income"));
    m_expenseTypeButton.signal_toggled().connect([this]
    {
        if (m_expenseTypeButton.get_active())
            selectType(BudgetEntryType::Expense);
    });

    m_incomeTypeButton.signal_toggled().connect([this]
    {
        if (m_incomeTypeButton.get_active())
            selectType(BudgetEntryType::Income);
    });

    m_expenseTypeButton.set_active(true);
    m_typeBox.append(m_expenseTypeButton);
    m_typeBox.append(m_incomeTypeButton);

    m_nameLabel.set_text(loc.translate("dialog.entry.name.label"));
    m_nameLabel.set_halign(Gtk::Align::START);
    m_nameEntry.set_placeholder_text(loc.translate("dialog.entry.name.placeholder"));

    m_categoryLabel.set_text(loc.translate("dialog.entry.category.label"));
    m_categoryLabel.set_halign(Gtk::Align::START);

    m_plannedAmountLabel.set_text(loc.translate("dialog.entry.planned.label"));
    m_plannedAmountLabel.set_halign(Gtk::Align::START);

    m_actualAmountLabel.set_text(loc.translate("dialog.entry.actual.label"));
    m_actualAmountLabel.set_halign(Gtk::Align::START);

    m_contentBox.set_margin(CONTENT_MARGIN);
    m_contentBox.append(m_typeLabel);
    m_contentBox.append(m_typeBox);
    m_contentBox.append(m_nameLabel);
    m_contentBox.append(m_nameEntry);
    m_contentBox.append(m_categoryLabel);
    m_contentBox.append(m_categoryCombo);
    m_contentBox.append(m_plannedAmountLabel);
    m_contentBox.append(m_plannedAmountEntry);
    m_contentBox.append(m_actualAmountLabel);
    m_contentBox.append(m_actualAmountEntry);

    get_content_area()->append(m_contentBox);

    add_button(loc.translate("dialog.button.cancel"), Gtk::ResponseType::CANCEL);
    add_button(loc.translate("dialog.button.ok"), Gtk::ResponseType::OK);

    set_default_response(Gtk::ResponseType::OK);
    m_nameEntry.signal_changed().connect([this] { refreshAcceptable(); });
    m_categoryCombo.signal_changed().connect([this] { refreshAcceptable(); });
    m_plannedAmountEntry.signalAmountChanged().connect([this] { refreshAcceptable(); });
    m_actualAmountEntry.signalAmountChanged().connect([this] { refreshAcceptable(); });
    m_nameEntry.set_activates_default(true);
    m_plannedAmountEntry.set_activates_default(true);
    m_actualAmountEntry.set_activates_default(true);
    set_default_size(DEFAULT_WIDTH, -1);
}

void BudgetEntryDialog::selectType(const BudgetEntryType type)
{
    m_expenseTypeButton.set_active(type == BudgetEntryType::Expense);
    m_incomeTypeButton.set_active(type == BudgetEntryType::Income);
}

BudgetEntryType BudgetEntryDialog::getSelectedType() const
{
    return m_incomeTypeButton.get_active() ? BudgetEntryType::Income : BudgetEntryType::Expense;
}

void BudgetEntryDialog::populateCategoryCombo(const std::string& selectedCategoryId)
{
    m_categoryCombo.remove_all();

    for (const auto& category : m_categories)
        m_categoryCombo.append(category.id, category.name);

    if (!selectedCategoryId.empty())
        m_categoryCombo.set_active_id(selectedCategoryId);
    else if (!m_categories.empty())
        m_categoryCombo.set_active(0);
}

bool BudgetEntryDialog::isAcceptable() const
{
    return !m_nameEntry.get_text().empty()
        && !m_categoryCombo.get_active_id().empty()
        && m_plannedAmountEntry.hasValidInput()
        && m_actualAmountEntry.hasValidInput();
}

void BudgetEntryDialog::refreshAcceptable()
{
    set_response_sensitive(Gtk::ResponseType::OK, isAcceptable());
}

std::optional<BudgetEntry> BudgetEntryDialog::getResult() const
{
    if (!isAcceptable())
        return std::nullopt;

    BudgetEntry entry;
    entry.name = m_nameEntry.get_text();
    entry.categoryId = m_categoryCombo.get_active_id();
    entry.type = getSelectedType();
    entry.plannedAmount = m_plannedAmountEntry.money();
    entry.actualAmount = m_actualAmountEntry.money();

    return entry;
}