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
#include "../../core/model/Category.hpp"

class BudgetEntryDialog : public Gtk::Dialog
{
public:
    BudgetEntryDialog(Window& parent, const std::vector<Category>& categories);
    BudgetEntryDialog(Window& parent, const std::vector<Category>& categories, const BudgetEntry& existing);

    [[nodiscard]] std::optional<BudgetEntry> getResult() const;

private:
    static constexpr int CONTENT_SPACING = 8;
    static constexpr int CONTENT_MARGIN = 16;
    static constexpr int DEFAULT_WIDTH = 400;

    const std::vector<Category>& m_categories;

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
    Gtk::Entry m_plannedAmountEntry;
    Gtk::Label m_actualAmountLabel;
    Gtk::Entry m_actualAmountEntry;

    void initLayout();
    void populateCategoryCombo(const std::string& selectedCategoryId);
    void selectType(BudgetEntryType type);

    [[nodiscard]] BudgetEntryType getSelectedType() const;

    [[nodiscard]] static std::optional<double> parseAmount(const Glib::ustring& text);
    [[nodiscard]] static std::string formatForEntry(double value);
};