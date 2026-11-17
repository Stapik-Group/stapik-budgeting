#include "BudgetView.hpp"

#include "../dialog/BudgetEntryDialog.hpp"

#include "stapik/locale/LocaleManager.hpp"

#include <gtkmm/window.h>

BudgetView::BudgetView() :
    Box(Gtk::Orientation::VERTICAL, 0),
    m_month(stapik::domain::YearMonth::current()),
    m_bottomBar(Gtk::Orientation::HORIZONTAL, 0)
{
    initLayout();
    initBottomBar();
    initNavigation();
    refreshView();

    LocaleManager::instance().signalLocaleChanged().connect([this] { updateLabels(); });
}

void BudgetView::initLayout()
{
    set_hexpand(true);
    set_vexpand(true);

    append(m_navBar);
    append(m_header);
    append(m_grid);
    append(m_bottomBar);

    m_grid.bindHeader(m_header);
}

void BudgetView::initBottomBar()
{
    m_bottomBar.set_halign(Gtk::Align::END);
    m_bottomBar.set_margin(BOTTOM_BAR_MARGIN);

    m_addEntryButton.add_css_class("suggested-action");
    updateLabels();

    m_bottomBar.append(m_addEntryButton);
}

void BudgetView::updateLabels()
{
    m_addEntryButton.set_label(LocaleManager::instance().translate("view.toolbar.addEntry"));
}

void BudgetView::initNavigation()
{
    m_navBar.signalPrevMonth().connect(sigc::mem_fun(*this, &BudgetView::navigatePrevMonth));
    m_navBar.signalNextMonth().connect(sigc::mem_fun(*this, &BudgetView::navigateNextMonth));

    m_addEntryButton.signal_clicked().connect([this] { showAddEntryDialog(); });
    m_grid.signalEditEntryRequested().connect([this](const std::size_t index) { showEditEntryDialog(index); });
}

void BudgetView::navigatePrevMonth()
{
    m_month = m_month.previous();
    refreshView();
}

void BudgetView::navigateNextMonth()
{
    m_month = m_month.next();
    refreshView();
}

void BudgetView::refreshView()
{
    m_navBar.updateDisplay(m_month);
    m_grid.displayMonth(m_month);
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
