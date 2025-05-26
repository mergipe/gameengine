#include "IO.h"
#include "Logger.h"
#include <sstream>

namespace Engine::IO
{
    std::vector<std::vector<int>> parseIntCsvFile(std::ifstream& file)
    {
        std::vector<std::vector<int>> rows{};
        std::string fileLine{};
        while (std::getline(file, fileLine)) {
            std::vector<int> row{};
            std::stringstream stream{fileLine};
            std::string value;
            while (std::getline(stream, value, ',')) {
                row.push_back(std::stoi(value));
            }
            rows.push_back(row);
        }
        return rows;
    }

    std::string readStringFromFile(const std::filesystem::path& filepath)
    {
        try {
            std::ifstream fileStream{filepath};
            std::stringstream stringStream{};
            stringStream << fileStream.rdbuf();
            fileStream.close();
            return stringStream.str();
        } catch (const std::exception& _) {
            Logger::error("Error reading string from file {}", filepath.c_str());
            return {};
        }
    }
} // namespace Engine::IO
