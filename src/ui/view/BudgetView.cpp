#include "BudgetView.hpp"

#include "../dialog/BudgetEntryDialog.hpp"
#include "../dialog/CategoriesManagerDialog.hpp"

#include <chrono>
#include <gtkmm/window.h>

BudgetView::BudgetView() :
    Box(Gtk::Orientation::VERTICAL, 0),
    m_toolBar(Gtk::Orientation::HORIZONTAL, TOOLBAR_SPACING)
{
    const auto today = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
    const std::chrono::year_month_day ymd{today};
    m_currentYear = static_cast<int>(ymd.year());
    m_currentMonth = static_cast<unsigned>(ymd.month());

    initLayout();
    initToolBar();
    initNavigation();
    refreshView();
}

void BudgetView::initLayout()
{
    set_hexpand(true);
    set_vexpand(true);

    append(m_navBar);
    append(m_toolBar);
    append(m_header);
    append(m_grid);
}

void BudgetView::initToolBar()
{
    m_toolBar.set_halign(Gtk::Align::CENTER);
    m_toolBar.set_margin(TOOLBAR_MARGIN);

    m_addEntryButton.set_label("+ Dodaj pozycję");
    m_manageCategoriesButton.set_label("Kategorie");

    m_toolBar.append(m_addEntryButton);
    m_toolBar.append(m_manageCategoriesButton);
}

void BudgetView::initNavigation()
{
    m_navBar.signalPrevMonth().connect(sigc::mem_fun(*this, &BudgetView::navigatePrevMonth));
    m_navBar.signalNextMonth().connect(sigc::mem_fun(*this, &BudgetView::navigateNextMonth));

    m_addEntryButton.signal_clicked().connect([this] { showAddEntryDialog(); });
    m_manageCategoriesButton.signal_clicked().connect([this] { showManageCategoriesDialog(); });
    m_grid.signalEditEntryRequested().connect([this](const std::size_t index) { showEditEntryDialog(index); });
}

void BudgetView::navigatePrevMonth()
{
    if (m_currentMonth == 1)
    {
        m_currentMonth = 12;
        --m_currentYear;
    }
    else
    {
        --m_currentMonth;
    }
    refreshView();
}

void BudgetView::navigateNextMonth()
{
    if (m_currentMonth == 12)
    {
        m_currentMonth = 1;
        ++m_currentYear;
    }
    else
    {
        ++m_currentMonth;
    }
    refreshView();
}

void BudgetView::refreshView()
{
    m_navBar.updateDisplay(m_currentYear, m_currentMonth);
    m_grid.displayMonth(m_currentYear, m_currentMonth);
}

BudgetGrid& BudgetView::getBudgetGrid()
{
    return m_grid;
}

void BudgetView::showAddEntryDialog()
{
    auto* parent = dynamic_cast<Gtk::Window*>(get_root());
    if (parent == nullptr)
        return;

    auto* dialog = new BudgetEntryDialog(*parent, m_grid.getCategories());

    dialog->signal_response().connect([this, dialog](const int responseId)
    {
        if (responseId == Gtk::ResponseType::OK)
        {
            if (auto result = dialog->getResult(); result.has_value())
                m_grid.addEntry(std::move(result.value()));
        }
        dialog->hide();
    });

    dialog->signal_hide().connect([dialog] { delete dialog; });
    dialog->show();
}

void BudgetView::showEditEntryDialog(const std::size_t index)
{
    auto* parent = dynamic_cast<Gtk::Window*>(get_root());
    if (parent == nullptr)
        return;

    const auto* existing = m_grid.getEntry(index);
    if (existing == nullptr)
        return;

    auto* dialog = new BudgetEntryDialog(*parent, m_grid.getCategories(), *existing);

    dialog->signal_response().connect([this, dialog, index](const int responseId)
    {
        if (responseId == Gtk::ResponseType::OK)
        {
            if (auto result = dialog->getResult(); result.has_value())
                m_grid.editEntry(index, std::move(result.value()));
        }
        dialog->hide();
    });

    dialog->signal_hide().connect([dialog] { delete dialog; });
    dialog->show();
}

void BudgetView::showManageCategoriesDialog()
{
    auto* parent = dynamic_cast<Gtk::Window*>(get_root());
    if (parent == nullptr)
        return;

    auto* dialog = new CategoriesManagerDialog(*parent, m_grid);
    dialog->signal_hide().connect([dialog] { delete dialog; });
    dialog->show();
}