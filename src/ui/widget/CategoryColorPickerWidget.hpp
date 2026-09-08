#pragma once

#include <gtkmm/box.h>
#include <gtkmm/togglebutton.h>
#include <array>

#include "../../core/model/CategoryColor.hpp"

class CategoryColorPickerWidget : public Gtk::Box
{
public:
    CategoryColorPickerWidget();
    void setSelectedColor(CategoryColor color);
    [[nodiscard]] CategoryColor getSelectedColor() const;
    sigc::signal<void(CategoryColor)>& signalColorSelected();

private:
    static constexpr int SWATCH_SPACING = 4;
    static constexpr int SWATCH_SIZE = 20;
    static constexpr std::size_t SWATCH_COUNT = 10;

    std::array<Gtk::ToggleButton, SWATCH_COUNT> m_swatches;
    CategoryColor m_selected = CategoryColor::Default;
    sigc::signal<void(CategoryColor)> m_signalColorSelected;

    void initLayout();
    void selectColor(CategoryColor color);
};