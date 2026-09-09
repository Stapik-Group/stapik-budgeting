#include "AmountFormatter.hpp"

#include "stapik/locale/Locale.hpp"
#include "stapik/locale/LocaleManager.hpp"

#include <algorithm>
#include <format>

std::string AmountFormatter::format(const double amount)
{
    auto text = std::format("{:.2f}", amount);
    const char decimalSeparator = LocaleManager::instance().getLocale() == Locale::EN ? '.' : ',';
    std::ranges::replace(text, '.', decimalSeparator);
    return text + " zł";
}

std::string AmountFormatter::format(const std::optional<double>& amount)
{
    return amount.has_value() ? format(amount.value()) : std::string("—");
}