#include "CategoryColorPickerWidget.hpp"

#include "../../core/util/CategoryColorUtils.hpp"

CategoryColorPickerWidget::CategoryColorPickerWidget() : Box(Gtk::Orientation::HORIZONTAL, SWATCH_SPACING)
{
    initLayout();
}

void CategoryColorPickerWidget::initLayout()
{
    const auto colors = CategoryColorUtils::allColors();
    for (std::size_t i = 0; i < colors.size(); ++i)
    {
        const auto color = colors[i];
        auto& swatch = m_swatches[i];
        swatch.set_size_request(SWATCH_SIZE, SWATCH_SIZE);
        swatch.add_css_class("color-swatch");
        swatch.add_css_class(CategoryColorUtils::toCssClass(color));
        swatch.signal_toggled().connect([this, color, &swatch]
        {
            if (swatch.get_active())
                selectColor(color);
        });
        append(swatch);
    }
    m_swatches.front().set_active(true);
}

void CategoryColorPickerWidget::selectColor(const CategoryColor color)
{
    m_selected = color;
    const auto colors = CategoryColorUtils::allColors();
    for (std::size_t i = 0; i < colors.size(); ++i)
        m_swatches[i].set_active(colors[i] == color);
    m_signalColorSelected.emit(color);
}

void CategoryColorPickerWidget::setSelectedColor(const CategoryColor color)
{
    selectColor(color);
}

CategoryColor CategoryColorPickerWidget::getSelectedColor() const
{
    return m_selected;
}

sigc::signal<void(CategoryColor)>& CategoryColorPickerWidget::signalColorSelected()
{
    return m_signalColorSelected;
}