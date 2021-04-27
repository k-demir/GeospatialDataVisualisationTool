#include "GraphicsLoader.h"
#include <fstream>
#include <iostream>


std::vector<float> GraphicsLoader::loadVertices(std::string&& filepath, unsigned int reserveCount) {
    std::vector<float> vertices;
    vertices.reserve(reserveCount);
    
    std::ifstream file(filepath);
    if (file.is_open()) {
        float x, y, z;
        while (file >> x >> y >> z) {
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
        file.close();
    }
    
    return vertices;
}


std::vector<unsigned int> GraphicsLoader::loadIndices(std::string&& filepath, unsigned int reserveCount) {
    std::vector<unsigned int> indices;
    indices.reserve(reserveCount);
    
    std::ifstream file(filepath);
    if (file.is_open()) {
        unsigned int i;
        while (file >> i) {
            indices.push_back(i);
        }
        file.close();
    }
    
    return indices;
}


std::vector<int> GraphicsLoader::loadRegionIds(std::string&& filepath) {
    std::vector<int> ids;
    ids.reserve(1000);
    
    std::ifstream file(filepath);
    if (file.is_open()) {
        int i;
        while (file >> i) {
            ids.push_back(i);
        }
        file.close();
    }
    
    return ids;
}
