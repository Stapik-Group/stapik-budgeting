cmake_minimum_required(VERSION 4.2)
project(stapikbudgeting VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_NAME_DISPLAY "Stapik Budgeting")
set(PROJECT_AUTHOR "Sebastian Smoliński")
set(PROJECT_GITHUB_URL "https://github.com/Stapik-Group/stapik-budgeting")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-4.0)

find_package(CURL REQUIRED)

include(FetchContent)
FetchContent_Declare(
        stapikcommon
        GIT_REPOSITORY https://github.com/Stapik-Group/stapik-common.git
        GIT_TAG v1.1.6
)
FetchContent_MakeAvailable(stapikcommon)

add_executable(stapikbudgeting
        src/main.cpp
        src/core/model/Category.cpp
        src/core/model/BudgetPeriod.cpp
        src/core/util/CategoryColorUtils.cpp
        src/core/util/CategoryIdGenerator.cpp
        src/core/util/AmountFormatter.cpp
        src/core/command/BudgetCommand.cpp
        src/core/command/BudgetEntryCommandBase.cpp
        src/core/command/IndexedBudgetEntryCommand.cpp
        src/core/command/AddEntryCommand.cpp
        src/core/command/DeleteEntryCommand.cpp
        src/core/command/EditEntryCommand.cpp
        src/core/command/MoveEntryCommand.cpp
        src/core/command/BudgetCommandHistory.cpp
        src/core/currency/CurrencyManager.cpp
        src/infrastructure/storage/BudgetStorage.cpp
        src/infrastructure/storage/BudgetSyncCoordinator.cpp
        src/infrastructure/storage/CurrencyCatalog.cpp
        src/infrastructure/network/CloudSchemaMigrationGuard.cpp
        src/ui/widget/BudgetRowWidget.cpp
        src/ui/widget/CategoryColorPickerWidget.cpp
        src/ui/widget/MainMenu.cpp
        src/ui/dialog/CategoryDialog.cpp
        src/ui/dialog/BudgetEntryDialog.cpp
        src/ui/dialog/CategoriesManagerDialog.cpp
        src/ui/view/BudgetHeader.cpp
        src/ui/view/BudgetNavBar.cpp
        src/ui/view/BudgetGrid.cpp
        src/ui/view/BudgetView.cpp
        src/ui/action/MenuActionHandler.cpp
        src/ui/window/MainWindow.cpp)

target_include_directories(stapikbudgeting PRIVATE ${GTKMM_INCLUDE_DIRS})
target_compile_options(stapikbudgeting PRIVATE ${GTKMM_CFLAGS_OTHER})
target_link_libraries(stapikbudgeting PRIVATE ${GTKMM_LIBRARIES} stapikcommon CURL::libcurl nlohmann_json::nlohmann_json)

add_custom_command(TARGET stapikbudgeting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:stapikbudgeting>/resources)

install(TARGETS stapikbudgeting
        RUNTIME DESTINATION lib/stapikbudgeting
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources/
        DESTINATION lib/stapikbudgeting/resources
)

install(PROGRAMS ${CMAKE_SOURCE_DIR}/packaging/stapikbudgeting-launcher.sh
        DESTINATION bin
        RENAME stapikbudgeting
)

install(FILES ${CMAKE_SOURCE_DIR}/packaging/stapikbudgeting.desktop
        DESTINATION share/applications
)

install(FILES ${CMAKE_SOURCE_DIR}/packaging/icon.png
        DESTINATION share/icons/hicolor/256x256/apps
        RENAME stapikbudgeting.png
)

set(CPACK_PACKAGE_NAME "stapikbudgeting")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR ${PROJECT_AUTHOR})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A retro-styled desktop budgeting application")
set(CPACK_PACKAGE_CONTACT "sebastian.smolinski6@gmail.com")

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${PROJECT_AUTHOR} <sebastian.smolinski6@gmail.com>")
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)

include(CPack)