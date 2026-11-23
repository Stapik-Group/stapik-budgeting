cmake_minimum_required(VERSION 4.2)
project(stapikbudgeting VERSION 1.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-4.0)

include(FetchContent)
FetchContent_Declare(
        stapikcommon
        GIT_REPOSITORY https://github.com/Stapik-Group/stapik-common.git
        GIT_TAG v1.6.0
)
FetchContent_MakeAvailable(stapikcommon)

stapik_add_application(
        NAME stapikbudgeting
        DISPLAY_NAME "Stapik Budgeting"
        DESCRIPTION "A retro-styled desktop budgeting application"
        APPLICATION_ID pl.stapik.budgeting
        MAINTAINER "Sebastian Smoliński <sebastian.smolinski6@gmail.com>"
        HOMEPAGE "https://github.com/Stapik-Group/stapik-budgeting"
        DESKTOP_CATEGORIES Office
        RESOURCES_DIRECTORY resources
        PACKAGING_DIRECTORY packaging
        SOURCES
        src/main.cpp
        src/app/BudgetController.cpp
        src/core/model/BudgetPeriod.cpp
        src/core/model/BudgetAmountsMigration.cpp
        src/core/model/BudgetDocument.cpp
        src/core/model/ResolveCurrency.cpp
        src/core/util/MoneyText.cpp
        src/core/report/MonthReportBuilder.cpp
        src/core/report/MonthCsv.cpp
        src/core/command/BudgetDocumentCommand.cpp
        src/core/command/BudgetEntryCommandBase.cpp
        src/core/command/IndexedBudgetEntryCommand.cpp
        src/core/command/AddEntryCommand.cpp
        src/core/command/DeleteEntryCommand.cpp
        src/core/command/EditEntryCommand.cpp
        src/core/command/MoveEntryCommand.cpp
        src/core/command/AddCategoryCommand.cpp
        src/core/command/EditCategoryCommand.cpp
        src/core/command/DeleteCategoryCommand.cpp
        src/infrastructure/storage/BudgetDocumentStore.cpp
        src/infrastructure/storage/LegacyCurrencyPreference.cpp
        src/ui/widget/BudgetRowWidget.cpp
        src/ui/widget/CategoryColorPickerWidget.cpp
        src/ui/settings/BudgetCurrencySetting.cpp
        src/ui/widget/MainMenu.cpp
        src/ui/dialog/CategoryDialog.cpp
        src/ui/dialog/BudgetEntryDialog.cpp
        src/ui/dialog/CategoriesManagerDialog.cpp
        src/ui/view/BudgetHeader.cpp
        src/ui/view/BudgetNavBar.cpp
        src/ui/view/BudgetGrid.cpp
        src/ui/view/BudgetView.cpp
        src/ui/action/MenuActionHandler.cpp
        src/ui/action/RestoreBackupAction.cpp
        src/ui/action/ArchiveActions.cpp
        src/ui/action/MonthExportActions.cpp
        src/ui/print/MonthPrintableDocument.cpp
        src/ui/window/CloudNotifications.cpp
        src/ui/window/MainWindow.cpp
        LINK_LIBRARIES PkgConfig::GTKMM nlohmann_json::nlohmann_json)

option(BUILD_TESTING "Build the unit tests" OFF)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
