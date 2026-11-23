#pragma once

#include "../../app/BudgetController.hpp"
#include "../../core/report/MonthReport.hpp"
#include "../../core/report/MonthReportBuilder.hpp"

#include <gtkmm/applicationwindow.h>

#include <optional>
#include <string>
#include <vector>

class MonthExportActions
{
public:
    MonthExportActions(Gtk::ApplicationWindow& window, BudgetController& controller);

    void registerActions();

private:
    Gtk::ApplicationWindow& m_window;
    BudgetController& m_controller;

    [[nodiscard]] std::optional<MonthReport> buildReport(MonthReportNumbers numbers) const;
    [[nodiscard]] static std::vector<std::string> columnTitles();
    [[nodiscard]] std::string defaultFileName(const std::string& extension) const;
    void showFailure(const std::string& reason) const;
    void onActionExportCsv() const;
    void onActionExportPdf() const;
    void onActionPrint() const;
};
