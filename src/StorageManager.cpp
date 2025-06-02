#include "StorageManager.h"
#include <fstream>

std::vector<std::string> StorageManager::loadResults(const std::string& filename, int maxCount) {
    std::vector<std::string> results;
    std::ifstream fin(filename);
    std::string line;
    while (std::getline(fin, line)) {
        if (!line.empty())
            results.push_back(line);
        if (results.size() >= maxCount)
            break;
    }
    return results;
}

void StorageManager::saveResults(const std::string& filename, const std::vector<std::string>& results) {
    std::ofstream fout(filename, std::ios::trunc);
    for (const auto& r : results)
        fout << r << '\n';
}
