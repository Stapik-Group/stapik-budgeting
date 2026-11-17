#include "ResolveCurrency.hpp"

#include "stapik/domain/CurrencyCatalog.hpp"
#include "stapik/log/Log.hpp"

#include <string>

stapik::domain::Currency resolveCurrency(const std::string_view code)
{
    if (const auto* currency = stapik::domain::CurrencyCatalog::instance().find(code))
        return *currency;

    stapik::log::warning("Currency '{}' is not in the catalog, its code is used as the symbol", code);
    return stapik::domain::Currency{ .code = std::string(code), .symbol = std::string(code) };
}
