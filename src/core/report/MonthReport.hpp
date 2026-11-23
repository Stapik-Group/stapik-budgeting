#pragma once

#include <string>
#include <vector>

struct MonthReportLine
{
    std::string category;
    std::string expense;
    std::string planned;
    std::string plannedRemaining;
    std::string actual;
    std::string actualRemaining;
};

struct MonthReport
{
    std::string title;
    std::string currencyCode;
    std::vector<std::string> columnTitles;
    std::vector<MonthReportLine> lines;
};
