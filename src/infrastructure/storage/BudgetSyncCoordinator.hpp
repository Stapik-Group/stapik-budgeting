#pragma once

#include "../../core/model/BudgetDocument.hpp"

#include "stapik/cloud/CloudStorageClient.hpp"

#include <optional>

class BudgetSyncCoordinator
{
public:
    [[nodiscard]] static BudgetDocument resolveOnConnect(const BudgetDocument& local, const CloudStorageClient& cloudClient);
    [[nodiscard]] static BudgetDocument pushLocalChange(const BudgetDocument& local, const CloudStorageClient& cloudClient);

private:
    [[nodiscard]] static BudgetDocument pushWithConflictResolution(
        const BudgetDocument& local,
        const CloudStorageClient& cloudClient,
        std::optional<std::chrono::system_clock::time_point> baseline);

    [[nodiscard]] static std::optional<BudgetDocument> fromCloudDocument(const CloudDocument& document);
    [[nodiscard]] static nlohmann::json toCloudJson(const BudgetDocument& document);
};
