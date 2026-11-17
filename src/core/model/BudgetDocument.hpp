#pragma once

#include "BudgetPeriod.hpp"
#include "stapik/domain/Category.hpp"
#include "stapik/domain/Currency.hpp"
#include "stapik/domain/YearMonth.hpp"

#include <chrono>
#include <cstddef>
#include <deque>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <utility>
#include <vector>

class BudgetDocument
{
public:
    using TimePoint = std::chrono::system_clock::time_point;

    static constexpr auto DEFAULT_CURRENCY_CODE = "PLN";

    BudgetDocument();
    explicit BudgetDocument(stapik::domain::Currency currency);

    [[nodiscard]] const std::vector<stapik::domain::Category>& categories() const { return m_categories; }
    [[nodiscard]] const stapik::domain::Category* findCategory(const std::string& categoryId) const;
    void addCategory(stapik::domain::Category category);
    void insertCategory(std::size_t index, stapik::domain::Category category);
    bool editCategory(const std::string& categoryId, stapik::domain::Category updated);
    void deleteCategory(const std::string& categoryId);

    [[nodiscard]] const BudgetPeriod* findPeriod(const stapik::domain::YearMonth& month) const;
    [[nodiscard]] BudgetPeriod& periodFor(const stapik::domain::YearMonth& month);

    [[nodiscard]] const stapik::domain::Currency& currency() const { return m_currency; }
    void changeCurrency(const stapik::domain::Currency& currency);

    [[nodiscard]] TimePoint lastUpdate() const { return m_lastUpdate; }
    [[nodiscard]] std::optional<TimePoint> lastKnownCloudUpdate() const { return m_lastKnownCloudUpdate; }
    void touch();
    [[nodiscard]] BudgetDocument withLastUpdate(TimePoint lastUpdate) const;
    [[nodiscard]] BudgetDocument withLastKnownCloudUpdate(TimePoint lastKnownCloudUpdate) const;

    [[nodiscard]] nlohmann::json toJson() const;
    [[nodiscard]] static BudgetDocument fromJson(const nlohmann::json& json);

private:
    std::vector<stapik::domain::Category> m_categories;
    std::deque<BudgetPeriod> m_periods;
    stapik::domain::Currency m_currency;
    TimePoint m_lastUpdate{};
    std::optional<TimePoint> m_lastKnownCloudUpdate;
};
