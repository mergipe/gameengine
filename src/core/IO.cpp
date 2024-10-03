#include <IO.h>

#include <sstream>

namespace Engine::IO {

std::vector<std::vector<int>> parseIntCsvFile(std::ifstream &file) {
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

} // namespace Engine::IO
