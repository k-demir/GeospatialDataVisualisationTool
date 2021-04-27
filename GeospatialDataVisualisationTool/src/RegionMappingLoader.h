#pragma once
#include <string>
#include <array>
#include <vector>
#include <unordered_map>


enum regionDataType {
    municipality_id = 0, municipality_fin = 1, municipality_swe = 2, municipality_eng = 3,
    region_id = 4, region_fin = 5, region_swe = 6, region_eng = 7,
    avi_id = 8, avi_fin = 9, avi_swe = 10, avi_eng = 11,
    ely_id = 12, ely_fin = 13, ely_swe = 14, ely_eng = 15,
    subregion_id = 16, subregion_fin = 17, subregion_swe = 18, subregion_eng = 19,
    nuts2_id = 20, nuts2_fin = 21, nuts2_swe = 22, nuts2_eng = 23,
    grouping_id = 24, grouping_fin = 25, grouping_swe = 26, grouping_eng = 27,
    languages_id = 28, languages_fin = 29, languages_swe = 30, languages_eng = 31
};


class RegionMappingLoader {
    std::vector<std::vector<std::string>> data_;

public:
    RegionMappingLoader(std::string&& filepath);
    std::unordered_map<std::string, int> getMunicipalityIdMapping(regionDataType fromType);
    std::array<int, 1000> getRegionIdArray(regionDataType fromType);
};
