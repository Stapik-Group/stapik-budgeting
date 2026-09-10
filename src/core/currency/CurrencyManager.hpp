#pragma once

#include "../model/Currency.hpp"

#include <sigc++/signal.h>
#include <filesystem>
#include <string>

class CurrencyManager
{
public:
    static CurrencyManager& instance(const std::string& appName = "");

    void setCurrency(const std::string& code);
    [[nodiscard]] const Currency& getCurrency() const;
    sigc::signal<void()>& signalCurrencyChanged();

private:
    explicit CurrencyManager(std::string appName);

    std::string m_appName;
    Currency m_currency;
    sigc::signal<void()> m_signalCurrencyChanged;

    void saveCurrency(const std::string& code) const;
    [[nodiscard]] std::string loadSavedCurrency() const;
    [[nodiscard]] std::filesystem::path currencyConfigPath() const;
};