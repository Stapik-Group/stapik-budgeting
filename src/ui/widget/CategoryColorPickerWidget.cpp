#include "CategoryColorPickerWidget.hpp"

#include "stapik/locale/LocaleManager.hpp"

CategoryColorPickerWidget::CategoryColorPickerWidget() : Box(Gtk::Orientation::HORIZONTAL, SWATCH_SPACING)
{
    initLayout();
}

void CategoryColorPickerWidget::initLayout()
{
    for (const auto color : stapik::domain::allCategoryColors())
    {
        auto* swatch = Gtk::make_managed<Gtk::ToggleButton>();
        swatch->set_size_request(SWATCH_SIZE, SWATCH_SIZE);
        swatch->add_css_class("stapik-color-swatch");
        swatch->add_css_class(stapik::domain::categoryColorCssClass(color));
        swatch->set_tooltip_text(LocaleManager::instance().translate(stapik::domain::categoryColorNameKey(color)));
        swatch->signal_toggled().connect([this, color, swatch]
        {
            if (swatch->get_active())
                selectColor(color);
        });
        m_swatches.push_back(swatch);
        append(*swatch);
    }
    selectColor(m_selected);
}

void CategoryColorPickerWidget::selectColor(const stapik::domain::CategoryColor color)
{
    m_selected = color;
    const auto colors = stapik::domain::allCategoryColors();
    for (std::size_t i = 0; i < colors.size(); ++i)
        m_swatches[i]->set_active(colors[i] == color);
    m_signalColorSelected.emit(color);
}

void CategoryColorPickerWidget::setSelectedColor(const stapik::domain::CategoryColor color)
{
    selectColor(color);
}

stapik::domain::CategoryColor CategoryColorPickerWidget::getSelectedColor() const
{
    return m_selected;
}

sigc::signal<void(stapik::domain::CategoryColor)>& CategoryColorPickerWidget::signalColorSelected()
{
    return m_signalColorSelected;
}
