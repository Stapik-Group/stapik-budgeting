#pragma once

#include <optional>
#include <vector>

#include <gtkmm/box.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/togglebutton.h>

#include "../../core/model/BudgetEntry.hpp"

#include "stapik/domain/Category.hpp"
#include "stapik/domain/Currency.hpp"
#include "stapik/ui/widget/MoneyEntry.hpp"

class BudgetEntryDialog : public Gtk::Dialog
{
public:
    BudgetEntryDialog(Window& parent, const std::vector<stapik::domain::Category>& categories,
        const stapik::domain::Currency& currency);
    BudgetEntryDialog(Window& parent, const std::vector<stapik::domain::Category>& categories,
        const stapik::domain::Currency& currency, const BudgetEntry& existing);

    [[nodiscard]] std::optional<BudgetEntry> getResult() const;

private:
    static constexpr int CONTENT_SPACING = 8;
    static constexpr int CONTENT_MARGIN = 16;
    static constexpr int DEFAULT_WIDTH = 400;

    const std::vector<stapik::domain::Category>& m_categories;

    Gtk::Box m_contentBox;
    Gtk::Label m_typeLabel;
    Gtk::Box m_typeBox;
    Gtk::ToggleButton m_expenseTypeButton;
    Gtk::ToggleButton m_incomeTypeButton;

    Gtk::Label m_nameLabel;
    Gtk::Entry m_nameEntry;
    Gtk::Label m_categoryLabel;
    Gtk::ComboBoxText m_categoryCombo;
    Gtk::Label m_plannedAmountLabel;
    MoneyEntry m_plannedAmountEntry;
    Gtk::Label m_actualAmountLabel;
    MoneyEntry m_actualAmountEntry;

    void initLayout();
    void populateCategoryCombo(const std::string& selectedCategoryId);
    void selectType(BudgetEntryType type);
    void refreshAcceptable();

    [[nodiscard]] BudgetEntryType getSelectedType() const;
    [[nodiscard]] bool isAcceptable() const;
};