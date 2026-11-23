#include "BudgetCurrencySetting.hpp"

#include "stapik/app/AppContext.hpp"
#include "stapik/domain/CurrencyCatalog.hpp"
#include "stapik/locale/LocaleManager.hpp"
#include "stapik/settings/AppSettings.hpp"

#include <algorithm>
#include <format>

BudgetCurrencySetting::BudgetCurrencySetting(BudgetGrid& budgetGrid) :
    m_budgetGrid(budgetGrid),
    m_setting(stapik::settings::appSettingsStore(stapik::app::AppContext::instance().info().internalName), "currency", budgetGrid.currency().code)
{
    m_setting.set(m_budgetGrid.currency().code);

    m_setting.signalChanged().connect([this](const std::string& code)
    {
        m_budgetGrid.changeCurrency(code);
        m_setting.set(m_budgetGrid.currency().code);
    });

    m_budgetGrid.signalCurrencyChanged().connect([this]
    {
        m_setting.set(m_budgetGrid.currency().code);
    });
}

void BudgetCurrencySetting::addTo(stapik::ui::SettingsLayout& layout)
{
    layout.addChoice<std::string>("menu.settings.currency", m_setting, [] { return currencyOptions(); });
}

std::vector<stapik::ui::ChoiceOption<std::string>> BudgetCurrencySetting::currencyOptions()
{
    const auto& loc = LocaleManager::instance();

    std::vector<stapik::ui::ChoiceOption<std::string>> options;
    for (const auto& currency : stapik::domain::CurrencyCatalog::instance().currencies())
        options.push_back({ .value = currency.code, .label = std::format("{} ({})", loc.translate(currency.nameKey()), currency.code) });

    std::ranges::sort(options, {}, &stapik::ui::ChoiceOption<std::string>::label);
    return options;
}
