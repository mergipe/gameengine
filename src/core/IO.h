#ifndef IO_H
#define IO_H

#include <fstream>
#include <vector>

namespace Engine::IO
{
    std::vector<std::vector<int>> parseIntCsvFile(std::ifstream& file);
}

#endif
