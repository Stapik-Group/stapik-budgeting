#include "BudgetStorage.hpp"

#include "../../core/util/CategoryColorUtils.hpp"
#include "stapik/sync/SyncEnvelope.hpp"
#include "stapik/sync/Timestamp.hpp"

#include <cstdlib>
#include <fstream>
#include <utility>

void BudgetStorage::save(const BudgetSnapshot& snapshot)
{
    const auto path = storagePath();
    std::filesystem::create_directories(path.parent_path());

    std::ofstream file(path);
    file << toJson(snapshot).dump(2);
}

BudgetSnapshot BudgetStorage::load()
{
    const auto path = storagePath();
    if (!std::filesystem::exists(path))
        return {};

    std::ifstream file(path);
    if (!file.is_open())
        return {};

    try
    {
        return fromJson(nlohmann::json::parse(file));
    }
    catch (const nlohmann::json::exception&)
    {
        return {};
    }
}

std::filesystem::path BudgetStorage::storagePath()
{
    const auto* home = std::getenv("HOME");
    if (home == nullptr)
        throw BudgetStorageException("No HOME in PATH");

    return std::filesystem::path(home) / ".local" / "share" / "stapikbudgeting" / "budget.json";
}

nlohmann::json BudgetStorage::categoryToJson(const Category& category)
{
    return {
        { "id", category.id },
        { "name", category.name },
        { "color", CategoryColorUtils::toString(category.color) }
    };
}

Category BudgetStorage::categoryFromJson(const nlohmann::json& json)
{
    return Category{
        .id = json.at("id").get<std::string>(),
        .name = json.at("name").get<std::string>(),
        .color = CategoryColorUtils::fromString(json.value("color", std::string{"default"}))
    };
}

nlohmann::json BudgetStorage::entryToJson(const BudgetEntry& entry)
{
    nlohmann::json json{
        { "name", entry.name },
        { "categoryId", entry.categoryId }
    };

    if (entry.plannedAmount.has_value())
        json["plannedAmount"] = entry.plannedAmount.value();
    if (entry.actualAmount.has_value())
        json["actualAmount"] = entry.actualAmount.value();

    return json;
}

BudgetEntry BudgetStorage::entryFromJson(const nlohmann::json& json)
{
    BudgetEntry entry;
    entry.name = json.at("name").get<std::string>();
    entry.categoryId = json.at("categoryId").get<std::string>();

    if (json.contains("plannedAmount"))
        entry.plannedAmount = json.at("plannedAmount").get<double>();
    if (json.contains("actualAmount"))
        entry.actualAmount = json.at("actualAmount").get<double>();

    return entry;
}

nlohmann::json BudgetStorage::periodToJson(const BudgetPeriod& period)
{
    nlohmann::json entries = nlohmann::json::array();
    for (const auto& entry : period.getEntries())
        entries.push_back(entryToJson(entry));

    return {
        { "year", period.getYear() },
        { "month", period.getMonth() },
        { "startingAmount", period.getStartingAmount() },
        { "entries", entries }
    };
}

BudgetPeriod BudgetStorage::periodFromJson(const nlohmann::json& json)
{
    BudgetPeriod period(
        json.at("year").get<int>(),
        json.at("month").get<unsigned>(),
        json.at("startingAmount").get<double>()
    );

    for (const auto& item : json.at("entries"))
        period.addEntry(entryFromJson(item));

    return period;
}

nlohmann::json BudgetStorage::toJson(const BudgetSnapshot& snapshot)
{
    nlohmann::json categories = nlohmann::json::array();
    for (const auto& category : snapshot.categories)
        categories.push_back(categoryToJson(category));

    nlohmann::json periods = nlohmann::json::array();
    for (const auto& period : snapshot.periods)
        periods.push_back(periodToJson(period));

    const nlohmann::json payload{
        { "categories", categories },
        { "periods", periods }
    };

    const stapik::sync::SyncEnvelope envelope{ .lastUpdate = snapshot.lastUpdate, .payload = payload };
    auto json = envelope.toJson();

    if (snapshot.lastKnownCloudUpdate.has_value())
        json["lastKnownCloudUpdate"] = stapik::sync::toIso8601(snapshot.lastKnownCloudUpdate.value());

    return json;
}

BudgetSnapshot BudgetStorage::fromJson(const nlohmann::json& json)
{
    try
    {
        const auto [lastUpdate, payload] = stapik::sync::SyncEnvelope::fromJson(json);

        std::vector<Category> categories;
        for (const auto& item : payload.value("categories", nlohmann::json::array()))
            categories.push_back(categoryFromJson(item));

        std::vector<BudgetPeriod> periods;
        for (const auto& item : payload.value("periods", nlohmann::json::array()))
            periods.push_back(periodFromJson(item));

        std::optional<std::chrono::system_clock::time_point> lastKnownCloudUpdate;
        if (json.contains("lastKnownCloudUpdate"))
            lastKnownCloudUpdate = stapik::sync::fromIso8601(json.at("lastKnownCloudUpdate").get<std::string>());

        return BudgetSnapshot{ .categories = std::move(categories), .periods = std::move(periods), .lastUpdate = lastUpdate, .lastKnownCloudUpdate = lastKnownCloudUpdate };
    }
    catch (const nlohmann::json::exception&)
    {
        return {};
    }
}