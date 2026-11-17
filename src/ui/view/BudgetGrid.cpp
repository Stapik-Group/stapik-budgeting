#include "BudgetGrid.hpp"
#include "BudgetHeader.hpp"

#include "../../core/command/AddEntryCommand.hpp"
#include "../../core/command/DeleteEntryCommand.hpp"
#include "../../core/command/EditEntryCommand.hpp"
#include "../../core/command/MoveEntryCommand.hpp"
#include "../../infrastructure/storage/BudgetSyncCoordinator.hpp"
#include "../widget/BudgetRowWidget.hpp"

#include "stapik/domain/CurrencyCatalog.hpp"
#include "stapik/locale/LocaleManager.hpp"
#include "stapik/log/Log.hpp"

#include <algorithm>
#include <chrono>
#include <memory>
#include <stdexcept>

#include <gtkmm/widget.h>

namespace
{
    constexpr int ROWS_BOX_SPACING = 2;
}

BudgetGrid::BudgetGrid() :
    Box(Gtk::Orientation::VERTICAL, 0),
    m_rowsBox(Gtk::Orientation::VERTICAL, ROWS_BOX_SPACING),
    m_document(m_store.load()),
    m_month(stapik::domain::YearMonth::current())
{
    initLayout();
    populateRows();

    LocaleManager::instance().signalLocaleChanged().connect([this] { populateRows(); });
}

void BudgetGrid::initLayout()
{
    using enum Gtk::SizeGroup::Mode;
    m_plannedSizeGroup = Gtk::SizeGroup::create(HORIZONTAL);
    m_actualSizeGroup = Gtk::SizeGroup::create(HORIZONTAL);
    m_actionsSizeGroup = Gtk::SizeGroup::create(HORIZONTAL);

    set_hexpand(true);
    set_vexpand(true);

    m_scrolledWindow.set_hexpand(true);
    m_scrolledWindow.set_vexpand(true);
    m_scrolledWindow.set_child(m_rowsBox);

    append(m_scrolledWindow);
}

void BudgetGrid::bindHeader(BudgetHeader& header) const
{
    m_plannedSizeGroup->add_widget(header.getPlannedGroup());
    m_actualSizeGroup->add_widget(header.getActualGroup());
    m_actionsSizeGroup->add_widget(header.getActionsSpacer());
}

BudgetPeriod& BudgetGrid::currentPeriod()
{
    return m_document.periodFor(m_month);
}

const BudgetPeriod* BudgetGrid::findCurrentPeriod() const
{
    return m_document.findPeriod(m_month);
}

stapik::domain::Category BudgetGrid::categoryFor(const std::string& categoryId) const
{
    if (const auto* category = m_document.findCategory(categoryId))
        return *category;

    return stapik::domain::Category{
        .id = {},
        .name = LocaleManager::instance().translate("category.uncategorized"),
        .color = stapik::domain::CategoryColor::Gray
    };
}

void BudgetGrid::displayMonth(const stapik::domain::YearMonth& month)
{
    m_month = month;
    populateRows();
}

void BudgetGrid::populateRows()
{
    while (auto* child = m_rowsBox.get_first_child())
        m_rowsBox.remove(*child);

    if (const auto* period = findCurrentPeriod())
        appendRows(*period);
}

void BudgetGrid::appendRows(const BudgetPeriod& period)
{
    const auto plannedRemaining = period.remainingPlanned();
    const auto actualRemaining = period.remainingActual();
    const auto& entries = period.getEntries();

    for (std::size_t i = 0; i < entries.size(); ++i)
    {
        const auto& entry = entries[i];
        const auto& category = categoryFor(entry.categoryId);

        auto* row = Gtk::make_managed<BudgetRowWidget>(entry, category, plannedRemaining[i], actualRemaining[i]);
        row->add_css_class(i % 2 == 0 ? "budget-row-even" : "budget-row-odd");

        m_plannedSizeGroup->add_widget(row->getPlannedGroup());
        m_actualSizeGroup->add_widget(row->getActualGroup());
        m_actionsSizeGroup->add_widget(row->getActionsBox());

        row->signalEditRequested().connect([this, i] { m_signalEditEntryRequested.emit(i); });
        row->signalDeleteRequested().connect([this, i] { deleteEntryAt(i); });
        row->signalMoveUpRequested().connect([this, i] { moveEntryUp(i); });
        row->signalMoveDownRequested().connect([this, i] { moveEntryDown(i); });

        m_rowsBox.append(*row);
    }
}

void BudgetGrid::deleteEntryAt(const std::size_t index)
{
    m_history.execute(std::make_unique<DeleteEntryCommand>(currentPeriod(), index));
    commit();
    populateRows();
}

void BudgetGrid::moveEntryUp(const std::size_t index)
{
    if (index == 0)
        return;

    m_history.execute(std::make_unique<MoveEntryCommand>(currentPeriod(), index, index - 1));
    commit();
    populateRows();
}

void BudgetGrid::moveEntryDown(const std::size_t index)
{
    auto& period = currentPeriod();
    if (index + 1 >= period.getEntries().size())
        return;

    m_history.execute(std::make_unique<MoveEntryCommand>(period, index, index + 1));
    commit();
    populateRows();
}

void BudgetGrid::addEntry(BudgetEntry entry)
{
    m_history.execute(std::make_unique<AddEntryCommand>(currentPeriod(), std::move(entry)));
    commit();
    populateRows();
}

void BudgetGrid::editEntry(const std::size_t index, BudgetEntry entry)
{
    m_history.execute(std::make_unique<EditEntryCommand>(currentPeriod(), index, std::move(entry)));
    commit();
    populateRows();
}

void BudgetGrid::undo()
{
    m_history.undo();
    commit();
    populateRows();
}

void BudgetGrid::redo()
{
    m_history.redo();
    commit();
    populateRows();
}

void BudgetGrid::commit()
{
    m_document.touch();

    if (m_cloudClient != nullptr)
    {
        stapik::log::info("[Cloud] Saving in cloud...");
        replaceDocument(BudgetSyncCoordinator::pushLocalChange(m_document, *m_cloudClient));
        stapik::log::info("[Cloud] Saved in cloud.");
    }

    m_store.save(m_document);
}

void BudgetGrid::replaceDocument(BudgetDocument document)
{
    const auto currencyChanged = document.currency() != m_document.currency();
    m_document = std::move(document);

    if (!currencyChanged)
        return;

    m_history.clear();
    m_signalCurrencyChanged.emit();
}

const stapik::domain::Currency& BudgetGrid::currency() const
{
    return m_document.currency();
}

void BudgetGrid::changeCurrency(const std::string& code)
{
    const auto* currency = stapik::domain::CurrencyCatalog::instance().find(code);
    if (currency == nullptr || *currency == m_document.currency())
        return;

    try
    {
        m_document.changeCurrency(*currency);
    }
    catch (const std::overflow_error& error)
    {
        stapik::log::warning("Cannot change the currency to {}: {}", code, error.what());
        return;
    }

    m_history.clear();
    commit();
    populateRows();
    m_signalCurrencyChanged.emit();
}

void BudgetGrid::setCloudClient(std::unique_ptr<CloudStorageClient> client)
{
    m_cloudClient = std::move(client);
    syncFromCloud();
}

void BudgetGrid::syncFromCloud()
{
    if (m_cloudClient == nullptr)
        return;

    replaceDocument(BudgetSyncCoordinator::resolveOnConnect(m_document, *m_cloudClient));

    m_store.save(m_document);
    populateRows();
}

void BudgetGrid::retrySync()
{
    syncFromCloud();
}

sigc::signal<void()>& BudgetGrid::signalCurrencyChanged() { return m_signalCurrencyChanged; }
sigc::signal<void()>& BudgetGrid::signalAddEntryRequested() { return m_signalAddEntryRequested; }
sigc::signal<void(std::size_t)>& BudgetGrid::signalEditEntryRequested() { return m_signalEditEntryRequested; }

const std::vector<stapik::domain::Category>& BudgetGrid::getCategories() const
{
    return m_document.categories();
}

const BudgetEntry* BudgetGrid::getEntry(const std::size_t index) const
{
    const auto* period = findCurrentPeriod();
    if (period == nullptr)
        return nullptr;

    const auto& entries = period->getEntries();
    return index < entries.size() ? &entries[index] : nullptr;
}

void BudgetGrid::addCategory(stapik::domain::Category category)
{
    m_document.addCategory(std::move(category));
    commit();
}

void BudgetGrid::editCategory(const std::string& categoryId, stapik::domain::Category updated)
{
    if (!m_document.editCategory(categoryId, std::move(updated)))
        return;

    commit();
    populateRows();
}

void BudgetGrid::deleteCategory(const std::string& categoryId)
{
    m_document.deleteCategory(categoryId);
    commit();
    populateRows();
}
