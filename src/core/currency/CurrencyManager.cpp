#include "CurrencyManager.hpp"
#include "../../infrastructure/storage/CurrencyCatalog.hpp"

#include "stapik/storage/AppPaths.hpp"

#include <fstream>
#include <utility>

CurrencyManager& CurrencyManager::instance(const std::string& appName)
{
    static CurrencyManager manager(appName);
    return manager;
}

CurrencyManager::CurrencyManager(std::string appName) :
    m_appName(std::move(appName))
{
    const auto code = loadSavedCurrency();

    if (const auto* currency = CurrencyCatalog::instance().findByCode(code); currency != nullptr)
        m_currency = *currency;
    else if (!CurrencyCatalog::instance().getCurrencies().empty())
        m_currency = CurrencyCatalog::instance().getCurrencies().front();
}

void CurrencyManager::setCurrency(const std::string& code)
{
    const auto* currency = CurrencyCatalog::instance().findByCode(code);

    if (currency == nullptr)
        return;

    m_currency = *currency;
    saveCurrency(code);
    m_signalCurrencyChanged.emit();
}

const Currency& CurrencyManager::getCurrency() const
{
    return m_currency;
}

sigc::signal<void()>& CurrencyManager::signalCurrencyChanged()
{
    return m_signalCurrencyChanged;
}

void CurrencyManager::saveCurrency(const std::string& code) const
{
    const auto path = currencyConfigPath();
    std::filesystem::create_directories(path.parent_path());

    std::ofstream file(path);
    file << code;
}

std::string CurrencyManager::loadSavedCurrency() const
{
    const auto path = currencyConfigPath();

    if (!std::filesystem::exists(path))
        return "PLN";

    std::ifstream file(path);

    if (!file.is_open())
        return "PLN";

    std::string code;
    file >> code;

    return code;
}

std::filesystem::path CurrencyManager::currencyConfigPath() const
{
    return AppPaths::userDataDir(m_appName) / "currency.txt";
}