#include "LegacyCurrencyPreference.hpp"

#include "../../AppInfo.hpp"

#include "stapik/storage/AppPaths.hpp"

#include <array>
#include <filesystem>
#include <fstream>

namespace
{
    constexpr auto FILE_NAME = "currency.txt";

    std::optional<std::string> readCode(const std::filesystem::path& file)
    {
        std::ifstream stream(file);
        std::string code;

        if (stream >> code)
            return code;

        return std::nullopt;
    }
}

std::optional<std::string> LegacyCurrencyPreference::read()
{
    const std::array locations{
        AppPaths::userDataDir(APP_NAME) / FILE_NAME,
        AppPaths::userDataDir("") / FILE_NAME
    };

    for (const auto& location : locations)
    {
        if (const auto code = readCode(location))
            return code;
    }

    return std::nullopt;
}
