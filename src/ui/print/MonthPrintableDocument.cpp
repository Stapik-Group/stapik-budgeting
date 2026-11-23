#include "MonthPrintableDocument.hpp"

#include <algorithm>
#include <cmath>
#include <format>
#include <string>
#include <utility>

namespace
{
    constexpr double TEXT_COLUMN_WEIGHT = 2.0;
    constexpr double AMOUNT_COLUMN_WEIGHT = 1.3;
    constexpr std::size_t FIRST_AMOUNT_COLUMN = 2;

    std::vector<std::string> cellsOf(const MonthReportLine& line)
    {
        return { line.category, line.expense, line.planned, line.plannedRemaining, line.actual, line.actualRemaining };
    }
}

MonthPrintableDocument::MonthPrintableDocument(MonthReport report) :
    m_report(std::move(report))
{
}

std::string MonthPrintableDocument::title() const
{
    return m_report.title;
}

int MonthPrintableDocument::rowsPerPage(const stapik::ui::PageSize& pageSize) const
{
    const auto usableHeight = pageSize.height - 2 * MARGIN - TITLE_BLOCK_HEIGHT - ROW_HEIGHT;
    return std::max(1, static_cast<int>(std::floor(usableHeight / ROW_HEIGHT)));
}

int MonthPrintableDocument::pageCount(const stapik::ui::PageSize& pageSize) const
{
    const auto rows = static_cast<int>(m_report.lines.size());
    const auto perPage = rowsPerPage(pageSize);
    return std::max(1, (rows + perPage - 1) / perPage);
}

std::vector<double> MonthPrintableDocument::columnEdges(const double usableWidth) const
{
    const auto columnCount = m_report.columnTitles.size();
    std::vector<double> weights;
    for (std::size_t column = 0; column < columnCount; ++column)
        weights.push_back(column < FIRST_AMOUNT_COLUMN ? TEXT_COLUMN_WEIGHT : AMOUNT_COLUMN_WEIGHT);

    double totalWeight = 0.0;
    for (const auto weight : weights)
        totalWeight += weight;

    std::vector<double> edges{ MARGIN };
    for (const auto weight : weights)
        edges.push_back(edges.back() + usableWidth * weight / totalWeight);

    return edges;
}

void MonthPrintableDocument::drawRow(const Cairo::RefPtr<Cairo::Context>& context, const std::vector<std::string>& cells,
                                     const double y, const std::vector<double>& edges) const
{
    for (std::size_t column = 0; column < cells.size(); ++column)
    {
        const auto left = edges[column];
        const auto right = edges[column + 1] - COLUMN_GAP;

        if (column < FIRST_AMOUNT_COLUMN)
        {
            context->move_to(left, y);
        }
        else
        {
            Cairo::TextExtents extents;
            context->get_text_extents(cells[column], extents);
            context->move_to(right - extents.width - extents.x_bearing, y);
        }

        context->show_text(cells[column]);
    }
}

void MonthPrintableDocument::drawPage(const Cairo::RefPtr<Cairo::Context>& context, const int pageIndex,
                                      const stapik::ui::PageSize& pageSize) const
{
    const auto edges = columnEdges(pageSize.width - 2 * MARGIN);
    const auto perPage = rowsPerPage(pageSize);

    context->set_source_rgb(0.0, 0.0, 0.0);
    context->select_font_face("Sans", Cairo::ToyFontFace::Slant::NORMAL, Cairo::ToyFontFace::Weight::BOLD);
    context->set_font_size(TITLE_FONT_SIZE);
    context->move_to(MARGIN, MARGIN + TITLE_FONT_SIZE);
    context->show_text(std::format("{} ({})", m_report.title, m_report.currencyCode));

    auto y = MARGIN + TITLE_BLOCK_HEIGHT + ROW_HEIGHT;
    context->set_font_size(TEXT_FONT_SIZE);
    drawRow(context, m_report.columnTitles, y, edges);

    context->set_line_width(0.5);
    context->move_to(MARGIN, y + 4);
    context->line_to(pageSize.width - MARGIN, y + 4);
    context->stroke();

    context->select_font_face("Sans", Cairo::ToyFontFace::Slant::NORMAL, Cairo::ToyFontFace::Weight::NORMAL);

    const auto first = static_cast<std::size_t>(pageIndex) * static_cast<std::size_t>(perPage);
    const auto last = std::min(m_report.lines.size(), first + static_cast<std::size_t>(perPage));

    for (auto i = first; i < last; ++i)
    {
        y += ROW_HEIGHT;
        drawRow(context, cellsOf(m_report.lines[i]), y, edges);
    }
}
