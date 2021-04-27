#include "RegionMappingLoader.h"
#include <fstream>


RegionMappingLoader::RegionMappingLoader(std::string&& filepath) {
    std::ifstream file(filepath);
    if (file.is_open()) {
        std::string line;
        int lineNum = 0;
        
        while (std::getline(file, line)) {
            data_.push_back(std::vector<std::string> {});
            int idx = 0, prevIdx = 0;
            
            while (idx < line.length()) {
                if (line.at(idx) == ';') {
                    data_[lineNum].push_back(line.substr(prevIdx, idx - prevIdx));
                    prevIdx = idx + 1;
                } else if (idx == line.length() - 1) {
                    data_[lineNum].push_back(line.substr(prevIdx, idx - prevIdx + 1));
                }
                idx++;
            }
            lineNum++;
        }
    }
    file.close();
}


std::unordered_map<std::string, int> RegionMappingLoader::getMunicipalityIdMapping(regionDataType fromType) {
    std::unordered_map<std::string, int> res;
    if (fromType == municipality_fin || fromType == municipality_eng || fromType == municipality_swe) {
        for (auto row : data_) {
            res[row[fromType]] = std::stoi(row[0]);
        }
    }
    return res;
}


std::array<int, 1000> RegionMappingLoader::getRegionIdArray(regionDataType fromType) {
    std::array<int, 1000> res;
    
    for (int i=0; i<res.size(); i++) {
        res[i] = -1;
    }
    for (int i=0; i<data_.size(); i++) {
        res[std::stoi(data_[i][0])] = std::stoi(data_[i][fromType]);
    }
    
    return res;
}
