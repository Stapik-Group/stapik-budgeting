#pragma once

#include "stapik/domain/Category.hpp"

#include <glibmm/object.h>

class CategoryItem : public Glib::Object
{
public:
    static Glib::RefPtr<CategoryItem> create(stapik::domain::Category category)
    {
        return Glib::make_refptr_for_instance(new CategoryItem(std::move(category)));
    }

    [[nodiscard]] const stapik::domain::Category& category() const { return m_category; }

private:
    explicit CategoryItem(stapik::domain::Category category) :
        m_category(std::move(category))
    {
    }

    stapik::domain::Category m_category;
};
