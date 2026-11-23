#include "BudgetController.hpp"

#include "../AppInfo.hpp"
#include "../core/command/AddCategoryCommand.hpp"
#include "../core/command/AddEntryCommand.hpp"
#include "../core/command/DeleteCategoryCommand.hpp"
#include "../core/command/DeleteEntryCommand.hpp"
#include "../core/command/EditCategoryCommand.hpp"
#include "../core/command/EditEntryCommand.hpp"
#include "../core/command/MoveEntryCommand.hpp"

#include "stapik/domain/CurrencyCatalog.hpp"
#include "stapik/locale/LocaleManager.hpp"
#include "stapik/log/Log.hpp"

#include <memory>
#include <stdexcept>

BudgetController::BudgetController() :
    m_document(m_store.load()),
    m_month(stapik::domain::YearMonth::current()),
    m_cloud(stapik::sync::defaultCloudSessionHooks(BUDGET_FILENAME))
{
    m_history.signalChanged().connect(sigc::mem_fun(*this, &BudgetController::onHistoryChanged));
    m_cloud.signalDocumentReplaced().connect(sigc::mem_fun(*this, &BudgetController::onCloudDocumentReplaced));
    m_cloud.signalBaselineChanged().connect(sigc::mem_fun(*this, &BudgetController::onCloudBaselineChanged));

    syncWithCloud();
}

void BudgetController::setMonth(const stapik::domain::YearMonth& month)
{
    m_month = month;
}

const stapik::domain::YearMonth& BudgetController::month() const
{
    return m_month;
}

const BudgetDocument& BudgetController::document() const
{
    return m_document;
}

const stapik::document::DocumentFile<BudgetDocument>& BudgetController::documentFile() const
{
    return m_store.file();
}

const BudgetPeriod* BudgetController::currentPeriod() const
{
    return m_document.findPeriod(m_month);
}

const BudgetEntry* BudgetController::getEntry(const std::size_t index) const
{
    const auto* period = currentPeriod();
    if (period == nullptr)
        return nullptr;

    const auto& entries = period->getEntries();
    return index < entries.size() ? &entries[index] : nullptr;
}

const std::vector<stapik::domain::Category>& BudgetController::getCategories() const
{
    return m_document.categories();
}

stapik::domain::Category BudgetController::categoryFor(const std::string& categoryId) const
{
    if (const auto* category = m_document.findCategory(categoryId))
        return *category;

    return stapik::domain::Category{
        .id = {},
        .name = LocaleManager::instance().translate("category.uncategorized"),
        .color = stapik::domain::CategoryColor::Gray
    };
}

void BudgetController::addEntry(BudgetEntry entry)
{
    m_history.execute(std::make_unique<AddEntryCommand>(m_document, m_month, std::move(entry)));
}

void BudgetController::editEntry(const std::size_t index, BudgetEntry entry)
{
    if (!hasEntryAt(index))
        return;

    m_history.execute(std::make_unique<EditEntryCommand>(m_document, m_month, index, std::move(entry)));
}

void BudgetController::deleteEntryAt(const std::size_t index)
{
    if (!hasEntryAt(index))
        return;

    m_history.execute(std::make_unique<DeleteEntryCommand>(m_document, m_month, index));
}

void BudgetController::moveEntryUp(const std::size_t index)
{
    if (index == 0 || !hasEntryAt(index))
        return;

    m_history.execute(std::make_unique<MoveEntryCommand>(m_document, m_month, index, index - 1));
}

void BudgetController::moveEntryDown(const std::size_t index)
{
    if (!hasEntryAt(index + 1))
        return;

    m_history.execute(std::make_unique<MoveEntryCommand>(m_document, m_month, index, index + 1));
}

void BudgetController::addCategory(stapik::domain::Category category)
{
    m_history.execute(std::make_unique<AddCategoryCommand>(m_document, std::move(category)));
}

void BudgetController::editCategory(const std::string& categoryId, stapik::domain::Category updated)
{
    if (m_document.findCategory(categoryId) == nullptr)
        return;

    m_history.execute(std::make_unique<EditCategoryCommand>(m_document, categoryId, std::move(updated)));
}

void BudgetController::deleteCategory(const std::string& categoryId)
{
    if (m_document.findCategory(categoryId) == nullptr)
        return;

    m_history.execute(std::make_unique<DeleteCategoryCommand>(m_document, categoryId));
}

void BudgetController::restoreDocument(BudgetDocument document)
{
    const auto currencyChanged = document.currency() != m_document.currency();
    m_document = std::move(document);

    clearHistory();
    commit();
    m_signalChanged.emit();

    if (currencyChanged)
        m_signalCurrencyChanged.emit();
}

const stapik::domain::Currency& BudgetController::currency() const
{
    return m_document.currency();
}

void BudgetController::changeCurrency(const std::string& code)
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

    clearHistory();
    commit();
    m_signalChanged.emit();
    m_signalCurrencyChanged.emit();
}

bool BudgetController::connectToCloud(const CloudStorageConfig& config)
{
    return m_cloud.connectWith(config, m_document);
}

void BudgetController::syncWithCloud()
{
    m_cloud.syncNow(m_document);
}

sigc::signal<void()>& BudgetController::signalChanged() { return m_signalChanged; }
sigc::signal<void()>& BudgetController::signalCurrencyChanged() { return m_signalCurrencyChanged; }

void BudgetController::commit()
{
    m_document.touch();
    m_store.save(m_document);
    m_cloud.pushChange(m_document);
}

void BudgetController::onHistoryChanged()
{
    if (m_historySilenced)
        return;

    commit();
    m_signalChanged.emit();
}

void BudgetController::clearHistory()
{
    m_historySilenced = true;
    m_history.clear();
    m_historySilenced = false;
}

void BudgetController::replaceDocument(BudgetDocument document)
{
    const auto currencyChanged = document.currency() != m_document.currency();
    const auto contentReplaced = currencyChanged || document.lastUpdate() != m_document.lastUpdate();
    m_document = std::move(document);

    if (contentReplaced)
        clearHistory();

    if (currencyChanged)
        m_signalCurrencyChanged.emit();
}

void BudgetController::onCloudDocumentReplaced(const BudgetDocument& document)
{
    replaceDocument(document);
    m_store.save(m_document);
    m_signalChanged.emit();
}

void BudgetController::onCloudBaselineChanged(const BudgetDocument::TimePoint baseline)
{
    m_document = m_document.withLastKnownCloudUpdate(baseline);
}

bool BudgetController::hasEntryAt(const std::size_t index) const
{
    const auto* period = currentPeriod();
    return period != nullptr && index < period->getEntries().size();
}
