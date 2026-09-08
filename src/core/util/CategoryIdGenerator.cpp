#include "CategoryIdGenerator.hpp"

#include <cstdint>
#include <iomanip>
#include <random>
#include <format>

namespace
{
    constexpr int ID_HEX_WIDTH = 16;
}

std::string CategoryIdGenerator::generate()
{
    static std::mt19937_64 engine(std::random_device{}());
    std::uniform_int_distribution<std::uint64_t> distribution;

    return std::format("{:0{}x}", distribution(engine), ID_HEX_WIDTH);
}