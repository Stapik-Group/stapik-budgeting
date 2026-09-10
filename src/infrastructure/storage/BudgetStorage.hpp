#pragma once

#include "../../core/model/BudgetPeriod.hpp"
#include "../../core/model/Category.hpp"

#include <chrono>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <optional>
#include <stdexcept>
#include <vector>
#include <string>

class BudgetStorageException : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

struct BudgetSnapshot
{
    std::vector<Category> categories;
    std::vector<BudgetPeriod> periods;
    std::string currencyCode;
    std::chrono::system_clock::time_point lastUpdate;
    std::optional<std::chrono::system_clock::time_point> lastKnownCloudUpdate;
};

class BudgetStorage
{
public:
    static void save(const BudgetSnapshot& snapshot);
    [[nodiscard]] static BudgetSnapshot load();
    [[nodiscard]] static nlohmann::json toJson(const BudgetSnapshot& snapshot);
    [[nodiscard]] static BudgetSnapshot fromJson(const nlohmann::json& json);
private:
    static std::filesystem::path storagePath();
    static nlohmann::json categoryToJson(const Category& category);
    static Category categoryFromJson(const nlohmann::json& json);
    static nlohmann::json entryToJson(const BudgetEntry& entry);
    static BudgetEntry entryFromJson(const nlohmann::json& json);
    static nlohmann::json periodToJson(const BudgetPeriod& period);
    static BudgetPeriod periodFromJson(const nlohmann::json& json);
};