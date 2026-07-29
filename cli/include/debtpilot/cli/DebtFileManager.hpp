#pragma once

#include "debtpilot/cli/DebtFileData.hpp"

#include <cstddef>
#include <filesystem>

namespace debtpilot::cli
{
    class DebtFileManager
    {
        public:
            static void generateTemplate(std::size_t debtCount, const std::filesystem::path& outputPath);

            [[nodiscard]] static DebtFileData load(const std::filesystem::path& inputPath);

        private:


    };
}