#pragma once

#include <optional>
#include <gtkmm/box.h>
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>

#include "../../core/model/Category.hpp"
#include "../widget/CategoryColorPickerWidget.hpp"

class CategoryDialog : public Gtk::Dialog
{
public:
    explicit CategoryDialog(Window& parent);
    CategoryDialog(Window& parent, const Category& existing);

    [[nodiscard]] std::optional<Category> getResult() const;
private:
    static constexpr int CONTENT_SPACING = 8;
    static constexpr int CONTENT_MARGIN = 16;
    static constexpr int DEFAULT_WIDTH = 360;

    std::optional<std::string> m_existingId;

    Gtk::Box m_contentBox;
    Gtk::Label m_nameLabel;
    Gtk::Entry m_nameEntry;
    Gtk::Label m_colorLabel;
    CategoryColorPickerWidget m_colorPicker;

    void initLayout();
};