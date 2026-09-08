#include "MenuActionHandler.hpp"

#include "../dialog/CategoriesManagerDialog.hpp"

#include <gtkmm/application.h>

MenuActionHandler::MenuActionHandler(Gtk::ApplicationWindow& window, BudgetGrid& budgetGrid):
    m_window(window),
    m_budgetGrid(budgetGrid) {}

void MenuActionHandler::registerActions()
{
    m_window.add_action("quit", sigc::mem_fun(*this, &MenuActionHandler::onActionQuit));
    m_window.add_action("undo", sigc::mem_fun(*this, &MenuActionHandler::onActionUndo));
    m_window.add_action("redo", sigc::mem_fun(*this, &MenuActionHandler::onActionRedo));
    m_window.add_action("manageCategories", sigc::mem_fun(*this, &MenuActionHandler::onActionManageCategories));
}

void MenuActionHandler::onActionQuit() const
{
    m_window.get_application()->quit();
}

void MenuActionHandler::onActionUndo() const
{
    m_budgetGrid.undo();
}

void MenuActionHandler::onActionRedo() const
{
    m_budgetGrid.redo();
}

void MenuActionHandler::onActionManageCategories() const
{
    auto* dialog = new CategoriesManagerDialog(m_window, m_budgetGrid);
    dialog->signal_hide().connect([dialog] { delete dialog; });
    dialog->show();
}