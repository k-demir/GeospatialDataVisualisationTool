#pragma once
#include <string>
#include <vector>


class GraphicsLoader {
public:
    static std::vector<float> loadVertices(std::string&& filepath, unsigned int reserveCount);
    static std::vector<unsigned int> loadIndices(std::string&& filepath, unsigned int reserveCount);
    static std::vector<int> loadRegionIds(std::string&& filepath);
};
