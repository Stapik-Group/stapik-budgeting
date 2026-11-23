include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
        FIND_PACKAGE_ARGS NAMES GTest
)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

set(BUDGETING_SOURCES "${PROJECT_SOURCE_DIR}/src")

add_executable(stapikbudgeting_tests
        core/BudgetAmountsMigrationTest.cpp
        core/BudgetCommandsTest.cpp
        core/BudgetDocumentTest.cpp
        core/BudgetPeriodTest.cpp
        report/MonthCsvTest.cpp
        report/MonthReportBuilderTest.cpp
        ${BUDGETING_SOURCES}/core/model/BudgetPeriod.cpp
        ${BUDGETING_SOURCES}/core/model/BudgetAmountsMigration.cpp
        ${BUDGETING_SOURCES}/core/model/BudgetDocument.cpp
        ${BUDGETING_SOURCES}/core/model/ResolveCurrency.cpp
        ${BUDGETING_SOURCES}/core/command/BudgetDocumentCommand.cpp
        ${BUDGETING_SOURCES}/core/command/BudgetEntryCommandBase.cpp
        ${BUDGETING_SOURCES}/core/command/IndexedBudgetEntryCommand.cpp
        ${BUDGETING_SOURCES}/core/command/AddEntryCommand.cpp
        ${BUDGETING_SOURCES}/core/command/DeleteEntryCommand.cpp
        ${BUDGETING_SOURCES}/core/command/EditEntryCommand.cpp
        ${BUDGETING_SOURCES}/core/command/MoveEntryCommand.cpp
        ${BUDGETING_SOURCES}/core/report/MonthReportBuilder.cpp
        ${BUDGETING_SOURCES}/core/report/MonthCsv.cpp
)

target_link_libraries(stapikbudgeting_tests PRIVATE
        stapik::common
        PkgConfig::GTKMM
        nlohmann_json::nlohmann_json
        GTest::gtest_main
)

target_compile_options(stapikbudgeting_tests PRIVATE -Wall -Wextra -Wpedantic)

gtest_discover_tests(stapikbudgeting_tests)
