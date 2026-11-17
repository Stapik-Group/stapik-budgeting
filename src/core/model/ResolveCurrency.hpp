#pragma once

#include "stapik/domain/Currency.hpp"

#include <string_view>

[[nodiscard]] stapik::domain::Currency resolveCurrency(std::string_view code);
