#include "MoneyText.hpp"

#include "stapik/domain/MoneyFormatter.hpp"
#include "stapik/locale/LocaleManager.hpp"

std::string MoneyText::format(const stapik::domain::Money& money)
{
    return stapik::domain::formatMoney(money, LocaleManager::instance().languageCode());
}

std::string MoneyText::format(const std::optional<stapik::domain::Money>& money)
{
    return money.has_value() ? format(*money) : std::string("—");
}
