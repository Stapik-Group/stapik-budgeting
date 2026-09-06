cmake_minimum_required(VERSION 4.2)
project(stapikbudgeting VERSION 0.1.0 LANGUAGES CXX)

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
        src/core/command/BudgetCommand.cpp
        src/core/command/BudgetEntryCommandBase.cpp
        src/core/command/IndexedBudgetEntryCommand.cpp
        src/core/command/AddEntryCommand.cpp
        src/core/command/DeleteEntryCommand.cpp
        src/core/command/EditEntryCommand.cpp
        src/core/command/MoveEntryCommand.cpp
        src/core/command/BudgetCommandHistory.cpp
        src/ui/window/MainWindow.cpp)

target_include_directories(stapikbudgeting PRIVATE ${GTKMM_INCLUDE_DIRS})
target_compile_options(stapikbudgeting PRIVATE ${GTKMM_CFLAGS_OTHER})
target_link_libraries(stapikbudgeting PRIVATE ${GTKMM_LIBRARIES} stapikcommon CURL::libcurl nlohmann_json::nlohmann_json)