#include "BudgetDocumentStore.hpp"

#include "../../AppInfo.hpp"

#include "stapik/log/Log.hpp"
#include "stapik/storage/AppPaths.hpp"
#include "stapik/storage/PathText.hpp"
#include "stapik/storage/Quarantine.hpp"
#include "stapik/time/Clock.hpp"

#include <utility>

namespace
{
    constexpr int LEGACY_SCHEMA_VERSION = 0;
    constexpr int CURRENT_SCHEMA_VERSION = 1;
    constexpr int BACKUP_COUNT = 3;

    stapik::document::SchemaMigrator createMigrator()
    {
        stapik::document::SchemaMigrator migrator(CURRENT_SCHEMA_VERSION);

        migrator.addStep(LEGACY_SCHEMA_VERSION, [](nlohmann::json& root)
        {
            auto document = root.at("payload");
            document["lastUpdate"] = root.at("lastUpdate");
            root = std::move(document);
        });

        return migrator;
    }
}

BudgetDocumentStore::BudgetDocumentStore() :
    m_file(AppPaths::userDataDir(APP_NAME) / BUDGET_FILENAME, createMigrator())
{
    m_file.setBackupCount(BACKUP_COUNT);
}

BudgetDocument BudgetDocumentStore::load() const
{
    const auto result = m_file.load();

    switch (result.status)
    {
        using enum stapik::document::LoadStatus;
        case Loaded:
        {
            if (result.migrated)
                save(*result.document);

            return *result.document;
        }
        case NewerVersion:
        case MigrationFailed:
            moveUnusableFileAside();
            return {};
        case Missing:
        case Corrupted:
            return {};
    }

    return {};
}

void BudgetDocumentStore::save(const BudgetDocument& document) const
{
    if (const auto saved = m_file.save(document); !saved)
        stapik::log::error("Cannot save the budget: {}", saved.error().message());
}

void BudgetDocumentStore::moveUnusableFileAside() const
{
    const auto moved = stapik::storage::quarantineFile(m_file.filePath(), stapik::time::systemClock().now());

    if (moved)
        stapik::log::warning("The budget file cannot be used by this version, it was moved to {}", stapik::storage::pathText(*moved));
    else
        stapik::log::error("Cannot move the unusable budget file aside: {}", moved.error().message());
}
