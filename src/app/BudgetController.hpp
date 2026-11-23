#pragma once

#include "../core/model/BudgetDocument.hpp"
#include "../infrastructure/storage/BudgetDocumentStore.hpp"

#include "stapik/cloud/CloudStorageConfig.hpp"
#include "stapik/command/UndoStack.hpp"
#include "stapik/domain/Currency.hpp"
#include "stapik/domain/YearMonth.hpp"
#include "stapik/sync/CloudSession.hpp"

#include <sigc++/signal.h>
#include <sigc++/trackable.h>

#include <string>
#include <vector>

class BudgetController : public sigc::trackable
{
public:
    BudgetController();

    BudgetController(const BudgetController&) = delete;
    BudgetController& operator=(const BudgetController&) = delete;

    void setMonth(const stapik::domain::YearMonth& month);
    [[nodiscard]] const stapik::domain::YearMonth& month() const;

    [[nodiscard]] const BudgetPeriod* currentPeriod() const;
    [[nodiscard]] const BudgetEntry* getEntry(std::size_t index) const;
    [[nodiscard]] const std::vector<stapik::domain::Category>& getCategories() const;
    [[nodiscard]] stapik::domain::Category categoryFor(const std::string& categoryId) const;

    void addEntry(BudgetEntry entry);
    void editEntry(std::size_t index, BudgetEntry entry);
    void deleteEntryAt(std::size_t index);
    void moveEntryUp(std::size_t index);
    void moveEntryDown(std::size_t index);

    void addCategory(stapik::domain::Category category);
    void editCategory(const std::string& categoryId, stapik::domain::Category updated);
    void deleteCategory(const std::string& categoryId);

    [[nodiscard]] const stapik::domain::Currency& currency() const;
    void changeCurrency(const std::string& code);

    bool connectToCloud(const CloudStorageConfig& config);
    void syncWithCloud();
    [[nodiscard]] stapik::sync::CloudSession<BudgetDocument>& cloud() { return m_cloud; }

    [[nodiscard]] stapik::command::UndoStack& undoStack() { return m_history; }

    sigc::signal<void()>& signalChanged();
    sigc::signal<void()>& signalCurrencyChanged();

private:
    BudgetDocumentStore m_store;
    BudgetDocument m_document;
    stapik::domain::YearMonth m_month;

    stapik::command::UndoStack m_history;
    bool m_historySilenced = false;
    stapik::sync::CloudSession<BudgetDocument> m_cloud;

    sigc::signal<void()> m_signalChanged;
    sigc::signal<void()> m_signalCurrencyChanged;

    void commit();
    void onHistoryChanged();
    void clearHistory();
    void replaceDocument(BudgetDocument document);
    void onCloudDocumentReplaced(const BudgetDocument& document);
    void onCloudBaselineChanged(BudgetDocument::TimePoint baseline);
    [[nodiscard]] bool hasEntryAt(std::size_t index) const;
};
