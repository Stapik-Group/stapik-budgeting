#pragma once

#include "../../core/report/MonthReport.hpp"

#include "stapik/ui/print/PrintableDocument.hpp"

#include <string>
#include <vector>

class MonthPrintableDocument : public stapik::ui::IPrintableDocument
{
public:
    explicit MonthPrintableDocument(MonthReport report);

    [[nodiscard]] std::string title() const override;
    [[nodiscard]] int pageCount(const stapik::ui::PageSize& pageSize) const override;
    void drawPage(const Cairo::RefPtr<Cairo::Context>& context, int pageIndex, const stapik::ui::PageSize& pageSize) const override;

private:
    static constexpr double MARGIN = 40.0;
    static constexpr double TITLE_FONT_SIZE = 18.0;
    static constexpr double TEXT_FONT_SIZE = 10.0;
    static constexpr double ROW_HEIGHT = 18.0;
    static constexpr double TITLE_BLOCK_HEIGHT = 40.0;
    static constexpr double COLUMN_GAP = 8.0;

    MonthReport m_report;

    [[nodiscard]] int rowsPerPage(const stapik::ui::PageSize& pageSize) const;
    [[nodiscard]] std::vector<double> columnEdges(double usableWidth) const;
    void drawRow(const Cairo::RefPtr<Cairo::Context>& context, const std::vector<std::string>& cells, double y,
                 const std::vector<double>& edges) const;
};
