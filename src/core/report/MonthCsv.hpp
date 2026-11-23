#pragma once

#include "MonthReport.hpp"

#include "stapik/csv/CsvWriter.hpp"

#include <string_view>

class MonthCsv
{
public:
    [[nodiscard]] static stapik::csv::CsvWriter write(const MonthReport& report, std::string_view languageCode);
};
