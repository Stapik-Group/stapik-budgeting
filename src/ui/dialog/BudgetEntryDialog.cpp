#include "BudgetEntryDialog.hpp"

#include "stapik/locale/LocaleManager.hpp"

#include <algorithm>
#include <format>
#include <stdexcept>

BudgetEntryDialog::BudgetEntryDialog(Window& parent, const std::vector<Category>& categories) :
    Dialog(LocaleManager::instance().translate("dialog.entry.new.title"), parent, true),
    m_categories(categories),
    m_contentBox(Gtk::Orientation::VERTICAL, CONTENT_SPACING)
{
    initLayout();
    populateCategoryCombo(m_categories.empty() ? std::string{} : m_categories.front().id);
}

BudgetEntryDialog::BudgetEntryDialog(Window& parent, const std::vector<Category>& categories, const BudgetEntry& existing) :
    Dialog(LocaleManager::instance().translate("dialog.entry.edit.title"), parent, true),
    m_categories(categories),
    m_contentBox(Gtk::Orientation::VERTICAL, CONTENT_SPACING)
{
    initLayout();
    populateCategoryCombo(existing.categoryId);

    m_nameEntry.set_text(existing.name);

    if (existing.plannedAmount.has_value())
        m_plannedAmountEntry.set_text(formatForEntry(existing.plannedAmount.value()));
    if (existing.actualAmount.has_value())
        m_actualAmountEntry.set_text(formatForEntry(existing.actualAmount.value()));
}

void BudgetEntryDialog::initLayout()
{
    const auto& loc = LocaleManager::instance();

    m_nameLabel.set_text(loc.translate("dialog.entry.name.label"));
    m_nameLabel.set_halign(Gtk::Align::START);
    m_nameEntry.set_placeholder_text(loc.translate("dialog.entry.name.placeholder"));

    m_categoryLabel.set_text(loc.translate("dialog.entry.category.label"));
    m_categoryLabel.set_halign(Gtk::Align::START);

    m_plannedAmountLabel.set_text(loc.translate("dialog.entry.planned.label"));
    m_plannedAmountLabel.set_halign(Gtk::Align::START);
    m_plannedAmountEntry.set_placeholder_text(loc.translate("dialog.entry.planned.placeholder"));

    m_actualAmountLabel.set_text(loc.translate("dialog.entry.actual.label"));
    m_actualAmountLabel.set_halign(Gtk::Align::START);
    m_actualAmountEntry.set_placeholder_text(loc.translate("dialog.entry.actual.placeholder"));

    m_contentBox.set_margin(CONTENT_MARGIN);
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
    m_nameEntry.set_activates_default(true);
    m_plannedAmountEntry.set_activates_default(true);
    m_actualAmountEntry.set_activates_default(true);
    set_default_size(DEFAULT_WIDTH, -1);
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

std::optional<double> BudgetEntryDialog::parseAmount(const Glib::ustring& text)
{
    auto raw = text.raw();
    if (raw.empty())
        return std::nullopt;

    std::ranges::replace(raw, ',', '.');

    try
    {
        std::size_t consumed = 0;
        const double value = std::stod(raw, &consumed);

        if (consumed != raw.size())
            return std::nullopt;

        return value;
    }
    catch (const std::invalid_argument&)
    {
        return std::nullopt;
    }
    catch (const std::out_of_range&)
    {
        return std::nullopt;
    }
}

std::string BudgetEntryDialog::formatForEntry(const double value)
{
    auto text = std::format("{:.2f}", value);
    std::ranges::replace(text, '.', ',');
    return text;
}

std::optional<BudgetEntry> BudgetEntryDialog::getResult() const
{
    const auto name = m_nameEntry.get_text();
    if (name.empty())
        return std::nullopt;

    const auto categoryId = m_categoryCombo.get_active_id();
    if (categoryId.empty())
        return std::nullopt;

    BudgetEntry entry;
    entry.name = name;
    entry.categoryId = categoryId;
    entry.plannedAmount = parseAmount(m_plannedAmountEntry.get_text());
    entry.actualAmount = parseAmount(m_actualAmountEntry.get_text());

    return entry;
}