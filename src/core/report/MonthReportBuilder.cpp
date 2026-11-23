#include "MonthReportBuilder.hpp"

#include "stapik/domain/MoneyFormatter.hpp"

#include <utility>

MonthReportBuilder::MonthReportBuilder(CategoryResolver categoryOf, stapik::domain::Currency currency, std::string languageCode) :
    m_categoryOf(std::move(categoryOf)),
    m_currency(std::move(currency)),
    m_languageCode(std::move(languageCode))
{
}

MonthReport MonthReportBuilder::build(const BudgetPeriod& period, std::string title, std::vector<std::string> columnTitles,
                                      const MonthReportNumbers numbers) const
{
    MonthReport report;
    report.title = std::move(title);
    report.currencyCode = m_currency.code;
    report.columnTitles = std::move(columnTitles);

    const auto plannedRemaining = period.remainingPlanned();
    const auto actualRemaining = period.remainingActual();
    const auto& entries = period.getEntries();

    for (std::size_t i = 0; i < entries.size(); ++i)
    {
        const auto& entry = entries[i];

        report.lines.push_back({
            .category = m_categoryOf(entry.categoryId).name,
            .expense = entry.name,
            .planned = formatOptional(entry.plannedAmount, numbers),
            .plannedRemaining = formatMoney(plannedRemaining[i], numbers),
            .actual = formatOptional(entry.actualAmount, numbers),
            .actualRemaining = formatMoney(actualRemaining[i], numbers)
        });
    }

    return report;
}

std::string MonthReportBuilder::formatMoney(const stapik::domain::Money& money, const MonthReportNumbers numbers) const
{
    if (numbers == MonthReportNumbers::WithSymbol)
        return stapik::domain::formatMoney(money, m_languageCode);

    return stapik::domain::formatAmount(money.minorUnits(), m_currency.decimalPlaces, m_languageCode, false);
}

std::string MonthReportBuilder::formatOptional(const std::optional<stapik::domain::Money>& money, const MonthReportNumbers numbers) const
{
    return money ? formatMoney(*money, numbers) : std::string{};
}
