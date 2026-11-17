#include "BudgetAmountsMigration.hpp"

#include "../../core/model/BudgetAmountKeys.hpp"
#include "../../core/model/BudgetDocument.hpp"
#include "../../core/model/ResolveCurrency.hpp"

#include "stapik/domain/Money.hpp"

#include <string>

namespace
{
    void convertAmount(nlohmann::json& owner, const char* legacyKey, const char* minorKey, const stapik::domain::Currency& currency)
    {
        const auto legacy = owner.find(legacyKey);
        if (legacy == owner.end())
            return;

        const auto minorUnits = stapik::domain::Money::fromMajor(legacy->get<double>(), currency).minorUnits();
        owner.erase(legacyKey);
        owner[minorKey] = minorUnits;
    }
}

void BudgetAmountsMigration::toMinorUnits(nlohmann::json& document)
{
    const auto currency = resolveCurrency(document.value("currency", std::string{BudgetDocument::DEFAULT_CURRENCY_CODE}));

    if (!document.contains("periods"))
        return;

    for (auto& period : document.at("periods"))
    {
        convertAmount(period, BudgetAmountKeys::LEGACY_STARTING_AMOUNT, BudgetAmountKeys::STARTING_AMOUNT_MINOR, currency);

        if (!period.contains("entries"))
            continue;

        for (auto& entry : period.at("entries"))
        {
            convertAmount(entry, BudgetAmountKeys::LEGACY_PLANNED_AMOUNT, BudgetAmountKeys::PLANNED_AMOUNT_MINOR, currency);
            convertAmount(entry, BudgetAmountKeys::LEGACY_ACTUAL_AMOUNT, BudgetAmountKeys::ACTUAL_AMOUNT_MINOR, currency);
        }
    }
}
