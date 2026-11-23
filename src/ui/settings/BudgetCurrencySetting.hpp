#pragma once

#include "../../app/BudgetController.hpp"

#include "stapik/settings/ObservableSetting.hpp"
#include "stapik/ui/settings/SettingsLayout.hpp"

#include <string>
#include <vector>

class BudgetCurrencySetting
{
public:
    explicit BudgetCurrencySetting(BudgetController& controller);

    BudgetCurrencySetting(const BudgetCurrencySetting&) = delete;
    BudgetCurrencySetting& operator=(const BudgetCurrencySetting&) = delete;

    void addTo(stapik::ui::SettingsLayout& layout);

private:
    [[nodiscard]] static std::vector<stapik::ui::ChoiceOption<std::string>> currencyOptions();

    BudgetController& m_controller;
    stapik::settings::ObservableSetting<std::string> m_setting;
};
