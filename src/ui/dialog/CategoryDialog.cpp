#include "CategoryDialog.hpp"

CategoryDialog::CategoryDialog(Gtk::Window& parent) :
    Dialog("Nowa kategoria", parent, true),
    m_contentBox(Gtk::Orientation::VERTICAL, CONTENT_SPACING)
{
    initLayout();
}

CategoryDialog::CategoryDialog(Gtk::Window& parent, const Category& existing) :
    Dialog("Edytuj kategorię", parent, true),
    m_existingId(existing.id),
    m_contentBox(Gtk::Orientation::VERTICAL, CONTENT_SPACING)
{
    initLayout();
    m_nameEntry.set_text(existing.name);
    m_colorPicker.setSelectedColor(existing.color);
}

void CategoryDialog::initLayout()
{
    m_nameLabel.set_text("Nazwa");
    m_nameLabel.set_halign(Gtk::Align::START);
    m_nameEntry.set_placeholder_text("np. Rachunki");
    m_nameEntry.set_activates_default(true);

    m_colorLabel.set_text("Kolor");
    m_colorLabel.set_halign(Gtk::Align::START);

    m_contentBox.set_margin(CONTENT_MARGIN);
    m_contentBox.append(m_nameLabel);
    m_contentBox.append(m_nameEntry);
    m_contentBox.append(m_colorLabel);
    m_contentBox.append(m_colorPicker);

    get_content_area()->append(m_contentBox);

    add_button("Anuluj", Gtk::ResponseType::CANCEL);
    add_button("OK", Gtk::ResponseType::OK);

    set_default_response(Gtk::ResponseType::OK);
    set_default_size(DEFAULT_WIDTH, -1);
}

std::optional<Category> CategoryDialog::getResult() const
{
    const auto name = m_nameEntry.get_text();
    if (name.empty())
        return std::nullopt;

    if (m_existingId.has_value())
        return Category{ m_existingId.value(), name, m_colorPicker.getSelectedColor() };

    return Category::create(name, m_colorPicker.getSelectedColor());
}