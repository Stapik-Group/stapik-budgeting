#include "CurrencyCatalog.hpp"

#include "stapik/storage/AppPaths.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

const CurrencyCatalog& CurrencyCatalog::instance()
{
    static const CurrencyCatalog catalog;
    return catalog;
}

CurrencyCatalog::CurrencyCatalog()
{
    std::ifstream file(AppPaths::resourcesDir() / "currencies.json");

    if (!file.is_open())
        return;

    const auto json = nlohmann::json::parse(file, nullptr, false);

    if (!json.is_array())
        return;

    for (const auto& item : json)
    {
        if (!item.contains("code") || !item.contains("symbol"))
            continue;

        m_currencies.push_back({
            item["code"].get<std::string>(),
            item["symbol"].get<std::string>()
        });
    }
}

const std::vector<Currency>& CurrencyCatalog::getCurrencies() const
{
    return m_currencies;
}

const Currency* CurrencyCatalog::findByCode(const std::string& code) const
{
    for (const auto& currency : m_currencies)
    {
        if (currency.code == code)
            return &currency;
    }

    return nullptr;
}