#include "BudgetGrid.hpp"

#include "../../core/command/AddEntryCommand.hpp"
#include "../../core/command/DeleteEntryCommand.hpp"
#include "../../core/command/EditEntryCommand.hpp"
#include "../../core/command/MoveEntryCommand.hpp"
#include "../../infrastructure/storage/BudgetSyncCoordinator.hpp"

#include <algorithm>
#include <chrono>
#include <glib.h>
#include <gtkmm/widget.h>

namespace
{
    constexpr int ROWS_BOX_SPACING = 2;
}

BudgetGrid::BudgetGrid() :
    Box(Gtk::Orientation::VERTICAL, 0),
    m_rowsBox(Gtk::Orientation::VERTICAL, ROWS_BOX_SPACING)
{
    auto snapshot = BudgetStorage::load();
    m_categories = std::move(snapshot.categories);
    m_periods = std::move(snapshot.periods);
    m_lastUpdate = snapshot.lastUpdate;
    m_lastKnownCloudUpdate = snapshot.lastKnownCloudUpdate;

    const auto today = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
    const std::chrono::year_month_day ymd{today};
    m_currentYear = static_cast<int>(ymd.year());
    m_currentMonth = static_cast<unsigned>(ymd.month());

    initLayout();
    populateRows();
}

void BudgetGrid::initLayout()
{
    set_hexpand(true);
    set_vexpand(true);

    m_scrolledWindow.set_hexpand(true);
    m_scrolledWindow.set_vexpand(true);
    m_scrolledWindow.set_child(m_rowsBox);

    append(m_scrolledWindow);
}

BudgetPeriod& BudgetGrid::currentPeriod()
{
    if (auto* existing = const_cast<BudgetPeriod*>(findCurrentPeriod()))
        return *existing;

    m_periods.emplace_back(m_currentYear, m_currentMonth, 0.0);
    return m_periods.back();
}

const BudgetPeriod* BudgetGrid::findCurrentPeriod() const
{
    const auto it = std::ranges::find_if(m_periods, [this](const BudgetPeriod& period)
    {
        return period.getYear() == m_currentYear && period.getMonth() == m_currentMonth;
    });

    return it != m_periods.end() ? &(*it) : nullptr;
}

const Category& BudgetGrid::categoryFor(const std::string& categoryId) const
{
    static const Category fallback = Category::create("Bez kategorii", CategoryColor::Default);

    const auto it = std::ranges::find_if(m_categories, [&categoryId](const Category& category)
    {
        return category.id == categoryId;
    });

    return it != m_categories.end() ? *it : fallback;
}

void BudgetGrid::displayMonth(const int year, const unsigned month)
{
    m_currentYear = year;
    m_currentMonth = month;
    populateRows();
}

void BudgetGrid::populateRows()
{
    while (auto* child = m_rowsBox.get_first_child())
        m_rowsBox.remove(*child);

    const auto& period = currentPeriod();
    const auto plannedRemaining = period.remainingPlanned();
    const auto actualRemaining = period.remainingActual();
    const auto& entries = period.getEntries();

    for (std::size_t i = 0; i < entries.size(); ++i)
    {
        const auto& entry = entries[i];
        const auto& category = categoryFor(entry.categoryId);

        auto* row = Gtk::make_managed<BudgetRowWidget>(entry, category, plannedRemaining[i], actualRemaining[i]);

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
    touchLastUpdate();
    saveSnapshot();
    populateRows();
}

void BudgetGrid::moveEntryUp(const std::size_t index)
{
    if (index == 0)
        return;

    m_history.execute(std::make_unique<MoveEntryCommand>(currentPeriod(), index, index - 1));
    touchLastUpdate();
    saveSnapshot();
    populateRows();
}

void BudgetGrid::moveEntryDown(const std::size_t index)
{
    auto& period = currentPeriod();
    if (index + 1 >= period.getEntries().size())
        return;

    m_history.execute(std::make_unique<MoveEntryCommand>(period, index, index + 1));
    touchLastUpdate();
    saveSnapshot();
    populateRows();
}

void BudgetGrid::addEntry(BudgetEntry entry)
{
    m_history.execute(std::make_unique<AddEntryCommand>(currentPeriod(), std::move(entry)));
    touchLastUpdate();
    saveSnapshot();
    populateRows();
}

void BudgetGrid::editEntry(const std::size_t index, BudgetEntry entry)
{
    m_history.execute(std::make_unique<EditEntryCommand>(currentPeriod(), index, std::move(entry)));
    touchLastUpdate();
    saveSnapshot();
    populateRows();
}

void BudgetGrid::undo()
{
    m_history.undo();
    touchLastUpdate();
    saveSnapshot();
    populateRows();
}

void BudgetGrid::redo()
{
    m_history.redo();
    touchLastUpdate();
    saveSnapshot();
    populateRows();
}

void BudgetGrid::saveSnapshot()
{
    if (m_cloudClient != nullptr)
    {
        const BudgetSnapshot snapshot{ m_categories, m_periods, m_lastUpdate, m_lastKnownCloudUpdate };
        g_message("[Cloud] Saving in cloud...");

        const auto resolved = BudgetSyncCoordinator::pushLocalChange(snapshot, *m_cloudClient);
        m_categories = resolved.categories;
        m_periods = resolved.periods;
        m_lastUpdate = resolved.lastUpdate;
        m_lastKnownCloudUpdate = resolved.lastKnownCloudUpdate;

        g_message("[Cloud] Saved in cloud.");
        BudgetStorage::save(resolved);
        return;
    }

    BudgetStorage::save(BudgetSnapshot{ m_categories, m_periods, m_lastUpdate, m_lastKnownCloudUpdate });
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

    const BudgetSnapshot local{ m_categories, m_periods, m_lastUpdate, m_lastKnownCloudUpdate };
    const auto resolved = BudgetSyncCoordinator::resolveOnConnect(local, *m_cloudClient);

    m_categories = resolved.categories;
    m_periods = resolved.periods;
    m_lastUpdate = resolved.lastUpdate;
    m_lastKnownCloudUpdate = resolved.lastKnownCloudUpdate;

    BudgetStorage::save(resolved);
    populateRows();
}

void BudgetGrid::retrySync()
{
    syncFromCloud();
}

void BudgetGrid::touchLastUpdate()
{
    m_lastUpdate = std::chrono::system_clock::now();
}

sigc::signal<void()>& BudgetGrid::signalAddEntryRequested() { return m_signalAddEntryRequested; }
sigc::signal<void(std::size_t)>& BudgetGrid::signalEditEntryRequested() { return m_signalEditEntryRequested; }

const std::vector<Category>& BudgetGrid::getCategories() const
{
    return m_categories;
}

const BudgetEntry* BudgetGrid::getEntry(const std::size_t index) const
{
    const auto* period = findCurrentPeriod();
    if (period == nullptr)
        return nullptr;

    const auto& entries = period->getEntries();
    return index < entries.size() ? &entries[index] : nullptr;
}

void BudgetGrid::addCategory(Category category)
{
    m_categories.push_back(std::move(category));
    touchLastUpdate();
    saveSnapshot();
}

void BudgetGrid::editCategory(const std::string& categoryId, Category updated)
{
    const auto it = std::ranges::find_if(m_categories, [&categoryId](const Category& category)
    {
        return category.id == categoryId;
    });

    if (it == m_categories.end())
        return;

    updated.id = categoryId;
    *it = std::move(updated);

    touchLastUpdate();
    saveSnapshot();
    populateRows();
}

void BudgetGrid::deleteCategory(const std::string& categoryId)
{
    std::erase_if(m_categories, [&categoryId](const Category& category) { return category.id == categoryId; });

    touchLastUpdate();
    saveSnapshot();
    populateRows();
}