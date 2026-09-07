#pragma once

#include "../widget/BudgetRowWidget.hpp"
#include "../../core/command/BudgetCommandHistory.hpp"
#include "../../infrastructure/storage/BudgetStorage.hpp"

#include "stapik/cloud/CloudStorageClient.hpp"

#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <memory>

class BudgetGrid : public Gtk::Box
{
public:
    explicit BudgetGrid();

    void displayMonth(int year, unsigned month);
    void undo();
    void redo();
    void setCloudClient(std::unique_ptr<CloudStorageClient> client);
    void retrySync();

    sigc::signal<void()>& signalAddEntryRequested();
    sigc::signal<void(std::size_t)>& signalEditEntryRequested();

    void deleteEntryAt(std::size_t index);
    void moveEntryUp(std::size_t index);
    void moveEntryDown(std::size_t index);
    void addEntry(BudgetEntry entry);
    void editEntry(std::size_t index, BudgetEntry entry);

    [[nodiscard]] const std::vector<Category>& getCategories() const;
    [[nodiscard]] const BudgetEntry* getEntry(std::size_t index) const;

    void addCategory(Category category);
    void editCategory(const std::string& categoryId, Category updated);
    void deleteCategory(const std::string& categoryId);
private:
    Gtk::ScrolledWindow m_scrolledWindow;
    Gtk::Box m_rowsBox;

    std::vector<Category> m_categories;
    std::vector<BudgetPeriod> m_periods;
    int m_currentYear;
    unsigned m_currentMonth;
    std::chrono::system_clock::time_point m_lastUpdate{};
    std::optional<std::chrono::system_clock::time_point> m_lastKnownCloudUpdate;

    BudgetCommandHistory m_history;
    std::unique_ptr<CloudStorageClient> m_cloudClient;

    sigc::signal<void()> m_signalAddEntryRequested;
    sigc::signal<void(std::size_t)> m_signalEditEntryRequested;

    void initLayout();
    [[nodiscard]] BudgetPeriod& currentPeriod();
    [[nodiscard]] const BudgetPeriod* findCurrentPeriod() const;
    [[nodiscard]] const Category& categoryFor(const std::string& categoryId) const;
    void populateRows();
    void saveSnapshot();
    void syncFromCloud();
    void touchLastUpdate();
};