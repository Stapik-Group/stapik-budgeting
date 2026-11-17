#pragma once

#include "stapik/domain/Money.hpp"

#include <optional>
#include <string>

class MoneyText
{
public:
    static std::string format(const stapik::domain::Money& money);
    static std::string format(const std::optional<stapik::domain::Money>& money);
};
