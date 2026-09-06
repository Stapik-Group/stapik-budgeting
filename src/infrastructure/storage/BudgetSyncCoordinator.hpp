#pragma once

#include "BudgetStorage.hpp"
#include "stapik/cloud/CloudStorageClient.hpp"

#include <optional>

class BudgetSyncCoordinator
{
public:
    // Whole-document last-write-wins. No per-entry merge — single-user tool.
    [[nodiscard]] static BudgetSnapshot resolveOnConnect(const BudgetSnapshot& local, CloudStorageClient& cloudClient);
    [[nodiscard]] static BudgetSnapshot pushLocalChange(const BudgetSnapshot& local, CloudStorageClient& cloudClient);

private:
    [[nodiscard]] static BudgetSnapshot pushWithConflictResolution(
        const BudgetSnapshot& local,
        CloudStorageClient& cloudClient,
        std::optional<std::chrono::system_clock::time_point> baseline);

    [[nodiscard]] static BudgetSnapshot fromCloudDocument(const CloudDocument& document);
};