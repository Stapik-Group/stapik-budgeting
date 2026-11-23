#include "MonthExportActions.hpp"

#include "../print/MonthPrintableDocument.hpp"
#include "../../core/report/MonthCsv.hpp"

#include "stapik/locale/LocaleManager.hpp"
#include "stapik/ui/dialog/DialogUtils.hpp"
#include "stapik/ui/dialog/FileDialogs.hpp"
#include "stapik/ui/print/PdfExport.hpp"
#include "stapik/ui/print/Printing.hpp"

#include <format>
#include <memory>

namespace
{
    constexpr auto LAST_DIRECTORY_KEY = "export";
}

MonthExportActions::MonthExportActions(Gtk::ApplicationWindow& window, BudgetController& controller) :
    m_window(window),
    m_controller(controller)
{
}

void MonthExportActions::registerActions()
{
    m_window.add_action("exportMonthCsv", sigc::mem_fun(*this, &MonthExportActions::onActionExportCsv));
    m_window.add_action("exportMonthPdf", sigc::mem_fun(*this, &MonthExportActions::onActionExportPdf));
    m_window.add_action("printMonth", sigc::mem_fun(*this, &MonthExportActions::onActionPrint));
}

std::optional<MonthReport> MonthExportActions::buildReport(const MonthReportNumbers numbers) const
{
    const auto& loc = LocaleManager::instance();

    const auto* period = m_controller.currentPeriod();
    if (period == nullptr || period->getEntries().empty())
    {
        showMessageDialog(m_window, loc.translate("export.empty.header"), loc.translate("export.empty.secondary"));
        return std::nullopt;
    }

    const MonthReportBuilder builder(
        [this](const std::string& categoryId) { return m_controller.categoryFor(categoryId); },
        m_controller.currency(),
        loc.languageCode());

    return builder.build(*period, m_controller.month().label(loc), columnTitles(), numbers);
}

std::vector<std::string> MonthExportActions::columnTitles()
{
    const auto& loc = LocaleManager::instance();
    const auto planned = loc.translate("view.header.planned");
    const auto actual = loc.translate("view.header.actual");
    const auto remaining = loc.translate("view.header.remaining");

    return {
        loc.translate("view.header.category"),
        loc.translate("view.header.expense"),
        planned,
        std::format("{}: {}", planned, remaining),
        actual,
        std::format("{}: {}", actual, remaining)
    };
}

std::string MonthExportActions::defaultFileName(const std::string& extension) const
{
    const auto& month = m_controller.month();
    return std::format("budget-{}-{:02}.{}", month.year(), month.month(), extension);
}

void MonthExportActions::showFailure(const std::string& reason) const
{
    showMessageDialog(m_window, LocaleManager::instance().translate("export.failed.header"), reason);
}

void MonthExportActions::onActionExportCsv() const
{
    const auto report = buildReport(MonthReportNumbers::Plain);
    if (!report)
        return;

    const FileDialogOptions options{
        .title = LocaleManager::instance().translate("export.csv.title"),
        .initialName = defaultFileName("csv"),
        .lastDirectoryKey = LAST_DIRECTORY_KEY
    };

    showSaveFileDialog(m_window, options, [this, report = *report](const std::filesystem::path& path)
    {
        const auto saved = MonthCsv::write(report, LocaleManager::instance().languageCode()).saveTo(path);
        if (!saved)
            showFailure(saved.error().message());
    });
}

void MonthExportActions::onActionExportPdf() const
{
    const auto report = buildReport(MonthReportNumbers::WithSymbol);
    if (!report)
        return;

    const FileDialogOptions options{
        .title = LocaleManager::instance().translate("export.pdf.title"),
        .initialName = defaultFileName("pdf"),
        .lastDirectoryKey = LAST_DIRECTORY_KEY
    };

    showSaveFileDialog(m_window, options, [this, report = *report](const std::filesystem::path& path)
    {
        const auto exported = stapik::ui::exportToPdf(MonthPrintableDocument(report), path);
        if (!exported)
            showFailure(exported.error().message());
    });
}

void MonthExportActions::onActionPrint() const
{
    const auto report = buildReport(MonthReportNumbers::WithSymbol);
    if (!report)
        return;

    stapik::ui::printDocument(m_window, std::make_shared<const MonthPrintableDocument>(*report),
        [this](const std::string& reason) { showFailure(reason); });
}
