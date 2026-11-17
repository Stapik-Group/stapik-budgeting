#include "BudgetSyncCoordinator.hpp"

#include "BudgetAmountsMigration.hpp"

#include "stapik/cloud/CloudStorageException.hpp"
#include "stapik/log/Log.hpp"
#include "stapik/sync/SyncEnvelope.hpp"

#include <exception>

std::optional<BudgetDocument> BudgetSyncCoordinator::fromCloudDocument(const CloudDocument& document)
{
    try
    {
        const auto envelope = stapik::sync::SyncEnvelope::fromJson(document.content);
        auto payload = envelope.payload;
        BudgetAmountsMigration::toMinorUnits(payload);
        return BudgetDocument::fromJson(payload)
            .withLastUpdate(envelope.lastUpdate)
            .withLastKnownCloudUpdate(document.updatedAt);
    }
    catch (const std::exception& exception)
    {
        stapik::log::warning("Cannot read the cloud document: {}", exception.what());
        return std::nullopt;
    }
}

nlohmann::json BudgetSyncCoordinator::toCloudJson(const BudgetDocument& document)
{
    return stapik::sync::SyncEnvelope{ .lastUpdate = document.lastUpdate(), .payload = document.toJson() }.toJson();
}

BudgetDocument BudgetSyncCoordinator::pushWithConflictResolution(
    const BudgetDocument& local,
    const CloudStorageClient& cloudClient,
    const std::optional<std::chrono::system_clock::time_point> baseline)
{
    CloudWriteResult result;

    try
    {
        result = cloudClient.saveDocument(toCloudJson(local), baseline.value_or(std::chrono::system_clock::time_point{}));
    }
    catch (const CloudStorageException&)
    {
        stapik::log::debug("Cannot sync with cloud, will retry on next save.");
        return local;
    }

    if (!result.conflict)
        return local.withLastKnownCloudUpdate(result.document.updatedAt);

    if (result.document.updatedAt > local.lastUpdate())
        return fromCloudDocument(result.document).value_or(local);

    try
    {
        const auto [document, conflict] = cloudClient.saveDocument(toCloudJson(local), result.document.updatedAt);

        if (!conflict)
            return local.withLastKnownCloudUpdate(document.updatedAt);

        return fromCloudDocument(document).value_or(local);
    }
    catch (const CloudStorageException&)
    {
        stapik::log::debug("Cannot sync with cloud, will retry on next save.");
        return local;
    }
}

BudgetDocument BudgetSyncCoordinator::resolveOnConnect(const BudgetDocument& local, const CloudStorageClient& cloudClient)
{
    std::optional<CloudDocument> remote;

    try
    {
        remote = cloudClient.loadDocument();
    }
    catch (const CloudStorageException&)
    {
        stapik::log::debug("Cloud unreachable right now, keeping local data.");
        return local;
    }

    if (!remote.has_value())
        return pushWithConflictResolution(local, cloudClient, std::nullopt);

    if (remote->updatedAt > local.lastUpdate())
        return fromCloudDocument(remote.value()).value_or(local);

    return pushWithConflictResolution(local, cloudClient, remote->updatedAt);
}

BudgetDocument BudgetSyncCoordinator::pushLocalChange(const BudgetDocument& local, const CloudStorageClient& cloudClient)
{
    return pushWithConflictResolution(local, cloudClient, local.lastKnownCloudUpdate());
}
