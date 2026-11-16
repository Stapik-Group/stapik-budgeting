#include "BudgetDocument.hpp"

#include "../util/CategoryColorUtils.hpp"

#include "stapik/sync/Timestamp.hpp"
#include "stapik/time/Clock.hpp"

#include <algorithm>
#include <utility>

namespace
{
    auto periodMatcher(const int year, const unsigned month)
    {
        return [year, month](const BudgetPeriod& period)
        {
            return period.getYear() == year && period.getMonth() == month;
        };
    }

    nlohmann::json categoryToJson(const Category& category)
    {
        return {
            { "id", category.id },
            { "name", category.name },
            { "color", CategoryColorUtils::toString(category.color) }
        };
    }

    Category categoryFromJson(const nlohmann::json& json)
    {
        return Category{
            .id = json.at("id").get<std::string>(),
            .name = json.at("name").get<std::string>(),
            .color = CategoryColorUtils::fromString(json.value("color", std::string{"default"}))
        };
    }

    nlohmann::json entryToJson(const BudgetEntry& entry)
    {
        nlohmann::json json{
            { "name", entry.name },
            { "categoryId", entry.categoryId },
            { "type", entry.type == BudgetEntryType::Income ? "income" : "expense" }
        };

        if (entry.plannedAmount.has_value())
            json["plannedAmount"] = entry.plannedAmount.value();
        if (entry.actualAmount.has_value())
            json["actualAmount"] = entry.actualAmount.value();

        return json;
    }

    BudgetEntry entryFromJson(const nlohmann::json& json)
    {
        BudgetEntry entry;
        entry.name = json.at("name").get<std::string>();
        entry.categoryId = json.at("categoryId").get<std::string>();
        entry.type = json.value("type", std::string{"expense"}) == "income" ?
            BudgetEntryType::Income :
            BudgetEntryType::Expense;

        if (json.contains("plannedAmount"))
            entry.plannedAmount = json.at("plannedAmount").get<double>();
        if (json.contains("actualAmount"))
            entry.actualAmount = json.at("actualAmount").get<double>();

        return entry;
    }

    nlohmann::json periodToJson(const BudgetPeriod& period)
    {
        nlohmann::json entries = nlohmann::json::array();
        for (const auto& entry : period.getEntries())
            entries.push_back(entryToJson(entry));

        return {
            { "year", period.getYear() },
            { "month", period.getMonth() },
            { "startingAmount", period.getStartingAmount() },
            { "entries", entries }
        };
    }

    BudgetPeriod periodFromJson(const nlohmann::json& json)
    {
        BudgetPeriod period(
            json.at("year").get<int>(),
            json.at("month").get<unsigned>(),
            json.at("startingAmount").get<double>()
        );

        for (const auto& item : json.at("entries"))
            period.addEntry(entryFromJson(item));

        return period;
    }
}

const Category* BudgetDocument::findCategory(const std::string& categoryId) const
{
    const auto it = std::ranges::find(m_categories, categoryId, &Category::id);
    return it != m_categories.end() ? &*it : nullptr;
}

void BudgetDocument::addCategory(Category category)
{
    m_categories.push_back(std::move(category));
}

bool BudgetDocument::editCategory(const std::string& categoryId, Category updated)
{
    const auto it = std::ranges::find(m_categories, categoryId, &Category::id);
    if (it == m_categories.end())
        return false;

    updated.id = categoryId;
    *it = std::move(updated);
    return true;
}

void BudgetDocument::deleteCategory(const std::string& categoryId)
{
    std::erase_if(m_categories, [&categoryId](const Category& category) { return category.id == categoryId; });
}

const BudgetPeriod* BudgetDocument::findPeriod(const int year, const unsigned month) const
{
    const auto it = std::ranges::find_if(m_periods, periodMatcher(year, month));
    return it != m_periods.end() ? &*it : nullptr;
}

BudgetPeriod& BudgetDocument::periodFor(const int year, const unsigned month)
{
    if (const auto it = std::ranges::find_if(m_periods, periodMatcher(year, month)); it != m_periods.end())
        return *it;

    return m_periods.emplace_back(year, month, 0.0);
}

void BudgetDocument::touch()
{
    m_lastUpdate = stapik::time::systemClock().now();
}

BudgetDocument BudgetDocument::withLastUpdate(const TimePoint lastUpdate) const
{
    auto copy = *this;
    copy.m_lastUpdate = lastUpdate;
    return copy;
}

BudgetDocument BudgetDocument::withLastKnownCloudUpdate(const TimePoint lastKnownCloudUpdate) const
{
    auto copy = *this;
    copy.m_lastKnownCloudUpdate = lastKnownCloudUpdate;
    return copy;
}

nlohmann::json BudgetDocument::toJson() const
{
    nlohmann::json categories = nlohmann::json::array();
    for (const auto& category : m_categories)
        categories.push_back(categoryToJson(category));

    nlohmann::json periods = nlohmann::json::array();
    for (const auto& period : m_periods)
        periods.push_back(periodToJson(period));

    return {
        { "categories", categories },
        { "periods", periods },
        { "currency", m_currencyCode },
        { "lastUpdate", stapik::sync::toIso8601(m_lastUpdate) }
    };
}

BudgetDocument BudgetDocument::fromJson(const nlohmann::json& json)
{
    BudgetDocument document;

    for (const auto& item : json.value("categories", nlohmann::json::array()))
        document.m_categories.push_back(categoryFromJson(item));

    for (const auto& item : json.value("periods", nlohmann::json::array()))
        document.m_periods.push_back(periodFromJson(item));

    document.m_currencyCode = json.value("currency", std::string{DEFAULT_CURRENCY_CODE});

    if (json.contains("lastUpdate"))
        document.m_lastUpdate = stapik::sync::fromIso8601(json.at("lastUpdate").get<std::string>());

    return document;
}
