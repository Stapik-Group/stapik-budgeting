#pragma once

#include <gtkmm/box.h>
#include <gtkmm/togglebutton.h>
#include <vector>

#include "stapik/domain/CategoryColor.hpp"

class CategoryColorPickerWidget : public Gtk::Box
{
public:
    CategoryColorPickerWidget();
    void setSelectedColor(stapik::domain::CategoryColor color);
    [[nodiscard]] stapik::domain::CategoryColor getSelectedColor() const;
    sigc::signal<void(stapik::domain::CategoryColor)>& signalColorSelected();

private:
    static constexpr int SWATCH_SPACING = 4;
    static constexpr int SWATCH_SIZE = 20;

    std::vector<Gtk::ToggleButton*> m_swatches;
    stapik::domain::CategoryColor m_selected = stapik::domain::CategoryColor::Gray;
    sigc::signal<void(stapik::domain::CategoryColor)> m_signalColorSelected;

    void initLayout();
    void selectColor(stapik::domain::CategoryColor color);
};
