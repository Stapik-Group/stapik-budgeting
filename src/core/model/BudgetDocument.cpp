#include "BudgetDocument.hpp"

#include "BudgetAmountKeys.hpp"
#include "ResolveCurrency.hpp"

#include "stapik/domain/CategoryJson.hpp"
#include "stapik/log/Log.hpp"
#include "stapik/sync/Timestamp.hpp"
#include "stapik/time/Clock.hpp"

#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <utility>

namespace
{
    using stapik::domain::Category;
    using stapik::domain::CategoryColor;
    using stapik::domain::Currency;
    using stapik::domain::Money;
    using stapik::domain::YearMonth;

    constexpr auto LEGACY_DEFAULT_COLOR_ID = "default";

    auto periodMatcher(const YearMonth& month)
    {
        return [month](const BudgetPeriod& period)
        {
            return period.getMonth() == month;
        };
    }

    CategoryColor colorFromId(const std::string& colorId)
    {
        if (colorId == LEGACY_DEFAULT_COLOR_ID)
            return CategoryColor::Gray;

        if (const auto color = stapik::domain::categoryColorFromId(colorId))
            return *color;

        stapik::log::warning("Unknown category color '{}', using gray", colorId);
        return CategoryColor::Gray;
    }

    nlohmann::json categoryToJson(const Category& category)
    {
        return nlohmann::json(category);
    }

    Category categoryFromJson(const nlohmann::json& json)
    {
        return Category{
            .id = json.at("id").get<std::string>(),
            .name = json.at("name").get<std::string>(),
            .color = colorFromId(json.value("color", std::string{LEGACY_DEFAULT_COLOR_ID}))
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
            json[BudgetAmountKeys::PLANNED_AMOUNT_MINOR] = entry.plannedAmount->minorUnits();
        if (entry.actualAmount.has_value())
            json[BudgetAmountKeys::ACTUAL_AMOUNT_MINOR] = entry.actualAmount->minorUnits();

        return json;
    }

    std::optional<Money> optionalAmountFromJson(const nlohmann::json& json, const char* key, const Currency& currency)
    {
        if (!json.contains(key))
            return std::nullopt;

        return Money(json.at(key).get<std::int64_t>(), currency);
    }

    BudgetEntry entryFromJson(const nlohmann::json& json, const Currency& currency)
    {
        BudgetEntry entry;
        entry.name = json.at("name").get<std::string>();
        entry.categoryId = json.at("categoryId").get<std::string>();
        entry.type = json.value("type", std::string{"expense"}) == "income" ?
            BudgetEntryType::Income :
            BudgetEntryType::Expense;

        entry.plannedAmount = optionalAmountFromJson(json, BudgetAmountKeys::PLANNED_AMOUNT_MINOR, currency);
        entry.actualAmount = optionalAmountFromJson(json, BudgetAmountKeys::ACTUAL_AMOUNT_MINOR, currency);

        return entry;
    }

    nlohmann::json periodToJson(const BudgetPeriod& period)
    {
        nlohmann::json entries = nlohmann::json::array();
        for (const auto& entry : period.getEntries())
            entries.push_back(entryToJson(entry));

        return {
            { "year", period.getMonth().year() },
            { "month", period.getMonth().month() },
            { BudgetAmountKeys::STARTING_AMOUNT_MINOR, period.getStartingAmount().minorUnits() },
            { "entries", entries }
        };
    }

    BudgetPeriod periodFromJson(const nlohmann::json& json, const Currency& currency)
    {
        const auto month = YearMonth::tryCreate(json.at("year").get<int>(), json.at("month").get<int>());
        if (!month)
            throw std::invalid_argument("The budget period has an invalid year or month");

        BudgetPeriod period(*month, Money(json.at(BudgetAmountKeys::STARTING_AMOUNT_MINOR).get<std::int64_t>(), currency));

        for (const auto& item : json.at("entries"))
            period.addEntry(entryFromJson(item, currency));

        return period;
    }
}

BudgetDocument::BudgetDocument() :
    m_currency(resolveCurrency(DEFAULT_CURRENCY_CODE))
{
}

BudgetDocument::BudgetDocument(stapik::domain::Currency currency) :
    m_currency(std::move(currency))
{
}

const stapik::domain::Category* BudgetDocument::findCategory(const std::string& categoryId) const
{
    const auto it = std::ranges::find(m_categories, categoryId, &stapik::domain::Category::id);
    return it != m_categories.end() ? &*it : nullptr;
}

void BudgetDocument::addCategory(stapik::domain::Category category)
{
    m_categories.push_back(std::move(category));
}

void BudgetDocument::insertCategory(const std::size_t index, stapik::domain::Category category)
{
    if (index > m_categories.size())
        throw std::out_of_range("BudgetDocument::insertCategory: index out of range");

    m_categories.insert(m_categories.begin() + static_cast<std::ptrdiff_t>(index), std::move(category));
}

bool BudgetDocument::editCategory(const std::string& categoryId, stapik::domain::Category updated)
{
    const auto it = std::ranges::find(m_categories, categoryId, &stapik::domain::Category::id);
    if (it == m_categories.end())
        return false;

    updated.id = categoryId;
    *it = std::move(updated);
    return true;
}

void BudgetDocument::deleteCategory(const std::string& categoryId)
{
    std::erase_if(m_categories, [&categoryId](const stapik::domain::Category& category) { return category.id == categoryId; });
}

const BudgetPeriod* BudgetDocument::findPeriod(const stapik::domain::YearMonth& month) const
{
    const auto it = std::ranges::find_if(m_periods, periodMatcher(month));
    return it != m_periods.end() ? &*it : nullptr;
}

BudgetPeriod& BudgetDocument::periodFor(const stapik::domain::YearMonth& month)
{
    if (const auto it = std::ranges::find_if(m_periods, periodMatcher(month)); it != m_periods.end())
        return *it;

    return m_periods.emplace_back(month, Money::zero(m_currency));
}

void BudgetDocument::changeCurrency(const stapik::domain::Currency& currency)
{
    auto periods = m_periods;
    for (auto& period : periods)
        period.changeCurrency(currency);

    m_periods = std::move(periods);
    m_currency = currency;
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
        { "currency", m_currency.code },
        { "lastUpdate", stapik::sync::toIso8601(m_lastUpdate) }
    };
}

BudgetDocument BudgetDocument::fromJson(const nlohmann::json& json)
{
    const auto currency = resolveCurrency(json.value("currency", std::string{DEFAULT_CURRENCY_CODE}));
    BudgetDocument document(currency);

    for (const auto& item : json.value("categories", nlohmann::json::array()))
        document.m_categories.push_back(categoryFromJson(item));

    for (const auto& item : json.value("periods", nlohmann::json::array()))
        document.m_periods.push_back(periodFromJson(item, currency));

    if (json.contains("lastUpdate"))
        document.m_lastUpdate = stapik::sync::fromIso8601(json.at("lastUpdate").get<std::string>());

    return document;
}
