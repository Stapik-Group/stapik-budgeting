#pragma once

#include "BudgetPeriod.hpp"
#include "stapik/domain/Category.hpp"
#include "stapik/domain/YearMonth.hpp"

#include <chrono>
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

    [[nodiscard]] const std::vector<stapik::domain::Category>& categories() const { return m_categories; }
    [[nodiscard]] const stapik::domain::Category* findCategory(const std::string& categoryId) const;
    void addCategory(stapik::domain::Category category);
    bool editCategory(const std::string& categoryId, stapik::domain::Category updated);
    void deleteCategory(const std::string& categoryId);

    [[nodiscard]] const BudgetPeriod* findPeriod(const stapik::domain::YearMonth& month) const;
    [[nodiscard]] BudgetPeriod& periodFor(const stapik::domain::YearMonth& month);

    [[nodiscard]] const std::string& currencyCode() const { return m_currencyCode; }
    void setCurrencyCode(std::string currencyCode) { m_currencyCode = std::move(currencyCode); }

    [[nodiscard]] TimePoint lastUpdate() const { return m_lastUpdate; }
    [[nodiscard]] std::optional<TimePoint> lastKnownCloudUpdate() const { return m_lastKnownCloudUpdate; }
    void touch();
    [[nodiscard]] BudgetDocument withLastUpdate(TimePoint lastUpdate) const;
    [[nodiscard]] BudgetDocument withLastKnownCloudUpdate(TimePoint lastKnownCloudUpdate) const;

    [[nodiscard]] nlohmann::json toJson() const;
    [[nodiscard]] static BudgetDocument fromJson(const nlohmann::json& json);

private:
    static constexpr auto DEFAULT_CURRENCY_CODE = "PLN";

    std::vector<stapik::domain::Category> m_categories;
    std::deque<BudgetPeriod> m_periods;
    std::string m_currencyCode = DEFAULT_CURRENCY_CODE;
    TimePoint m_lastUpdate{};
    std::optional<TimePoint> m_lastKnownCloudUpdate;
};
