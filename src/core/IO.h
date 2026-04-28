#ifndef IO_H
#define IO_H

#include <filesystem>
#include <fstream>
#include <vector>

namespace Engine::IO
{
    std::vector<std::vector<int>> ParseIntCsvFile(std::ifstream& file);
    std::string ReadStringFromFile(const std::filesystem::path& filepath);
} // namespace Engine::IO

#endif
