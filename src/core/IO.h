#ifndef IO_H
#define IO_H

#include <filesystem>
#include <fstream>
#include <vector>

namespace Engine::IO
{
    std::vector<std::vector<int>> parseIntCsvFile(std::ifstream& file);
    std::string readStringFromFile(const std::filesystem::path& filepath);
} // namespace Engine::IO

#endif
