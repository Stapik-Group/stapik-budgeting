#include "MonthCsv.hpp"

stapik::csv::CsvWriter MonthCsv::write(const MonthReport& report, const std::string_view languageCode)
{
    stapik::csv::CsvWriter writer(stapik::csv::CsvOptions::forExcel(languageCode));
    writer.addRow(report.columnTitles);

    for (const auto& line : report.lines)
        writer.addRow({ line.category, line.expense, line.planned, line.plannedRemaining, line.actual, line.actualRemaining });

    return writer;
}
