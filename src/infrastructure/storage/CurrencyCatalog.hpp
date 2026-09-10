#pragma once

#include "../../core/model/Currency.hpp"
#include <vector>

class CurrencyCatalog
{
public:
    static const CurrencyCatalog& instance();

    [[nodiscard]] const std::vector<Currency>& getCurrencies() const;
    [[nodiscard]] const Currency* findByCode(const std::string& code) const;

private:
    CurrencyCatalog();

    std::vector<Currency> m_currencies;
};