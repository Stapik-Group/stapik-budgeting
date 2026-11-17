#pragma once

#include "stapik/domain/Currency.hpp"
#include "stapik/domain/Money.hpp"

[[nodiscard]] inline stapik::domain::Money restateIn(const stapik::domain::Money& money, const stapik::domain::Currency& currency)
{
    if (money.currency().decimalPlaces == currency.decimalPlaces)
        return stapik::domain::Money(money.minorUnits(), currency);

    return stapik::domain::Money::fromMajor(money.toMajor(), currency);
}
