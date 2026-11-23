#pragma once

#include "MonthReport.hpp"
#include "../model/BudgetPeriod.hpp"

#include "stapik/domain/Category.hpp"
#include "stapik/domain/Currency.hpp"

#include <functional>
#include <optional>
#include <string>
#include <vector>

enum class MonthReportNumbers
{
    Plain,
    WithSymbol
};

class MonthReportBuilder
{
public:
    using CategoryResolver = std::function<stapik::domain::Category(const std::string& categoryId)>;

    MonthReportBuilder(CategoryResolver categoryOf, stapik::domain::Currency currency, std::string languageCode);

    [[nodiscard]] MonthReport build(const BudgetPeriod& period, std::string title, std::vector<std::string> columnTitles,
                                    MonthReportNumbers numbers) const;

private:
    CategoryResolver m_categoryOf;
    stapik::domain::Currency m_currency;
    std::string m_languageCode;

    [[nodiscard]] std::string formatMoney(const stapik::domain::Money& money, MonthReportNumbers numbers) const;
    [[nodiscard]] std::string formatOptional(const std::optional<stapik::domain::Money>& money, MonthReportNumbers numbers) const;
};
