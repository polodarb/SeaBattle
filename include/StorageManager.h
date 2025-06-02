#ifndef SEABATTLE_STORAGEMANAGER_H
#define SEABATTLE_STORAGEMANAGER_H

#include <vector>
#include <string>

class StorageManager {
public:
    static std::vector<std::string> loadResults(const std::string& filename, int maxCount);
    static void saveResults(const std::string& filename, const std::vector<std::string>& results);
};

#endif // SEABATTLE_STORAGEMANAGER_H
