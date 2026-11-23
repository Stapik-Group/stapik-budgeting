#pragma once

#include "../../app/BudgetController.hpp"

#include "stapik/archive/AppArchive.hpp"

#include <gtkmm/applicationwindow.h>

#include <filesystem>

class ArchiveActions
{
public:
    ArchiveActions(Gtk::ApplicationWindow& window, BudgetController& controller);

    void registerActions();

private:
    Gtk::ApplicationWindow& m_window;
    BudgetController& m_controller;

    [[nodiscard]] stapik::archive::AppArchiveContext context() const;
    void onActionExport() const;
    void onActionImport() const;
    void exportTo(const std::filesystem::path& target) const;
    void importFrom(const std::filesystem::path& source) const;
};
