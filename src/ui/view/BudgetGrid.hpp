#pragma once

#include "../../core/command/BudgetCommandHistory.hpp"
#include "../../core/model/BudgetDocument.hpp"
#include "../../infrastructure/storage/BudgetDocumentStore.hpp"

#include "stapik/cloud/CloudStorageClient.hpp"

#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/sizegroup.h>
#include <memory>

#include "BudgetHeader.hpp"

class BudgetGrid : public Gtk::Box
{
public:
    explicit BudgetGrid();

    void displayMonth(const stapik::domain::YearMonth& month);
    void undo();
    void redo();
    void setCloudClient(std::unique_ptr<CloudStorageClient> client);
    void retrySync();
    void bindHeader(BudgetHeader& header) const;

    sigc::signal<void()>& signalAddEntryRequested();
    sigc::signal<void(std::size_t)>& signalEditEntryRequested();

    void deleteEntryAt(std::size_t index);
    void moveEntryUp(std::size_t index);
    void moveEntryDown(std::size_t index);
    void addEntry(BudgetEntry entry);
    void editEntry(std::size_t index, BudgetEntry entry);

    [[nodiscard]] const std::vector<stapik::domain::Category>& getCategories() const;
    [[nodiscard]] const BudgetEntry* getEntry(std::size_t index) const;

    void addCategory(stapik::domain::Category category);
    void editCategory(const std::string& categoryId, stapik::domain::Category updated);
    void deleteCategory(const std::string& categoryId);
private:
    Gtk::ScrolledWindow m_scrolledWindow;
    Box m_rowsBox;

    BudgetDocumentStore m_store;
    BudgetDocument m_document;
    stapik::domain::YearMonth m_month;

    Glib::RefPtr<Gtk::SizeGroup> m_plannedSizeGroup;
    Glib::RefPtr<Gtk::SizeGroup> m_actualSizeGroup;
    Glib::RefPtr<Gtk::SizeGroup> m_actionsSizeGroup;

    BudgetCommandHistory m_history;
    std::unique_ptr<CloudStorageClient> m_cloudClient;

    sigc::signal<void()> m_signalAddEntryRequested;
    sigc::signal<void(std::size_t)> m_signalEditEntryRequested;

    bool m_syncingCurrency = false;

    void initLayout();
    [[nodiscard]] BudgetPeriod& currentPeriod();
    [[nodiscard]] const BudgetPeriod* findCurrentPeriod() const;
    [[nodiscard]] stapik::domain::Category categoryFor(const std::string& categoryId) const;
    void populateRows();
    void appendRows(const BudgetPeriod& period);
    void commit();
    void syncFromCloud();
};
