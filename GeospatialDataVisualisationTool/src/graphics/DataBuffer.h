#pragma once
#include <array>
#include <utility>
#include <vector>
#include "State.h"


class DataBuffer {
    unsigned int idTex_;
    unsigned int idBuff_;
    static constexpr unsigned int count_ = 4020;
    static constexpr unsigned int municipalityRange_ = 993;
    std::array<float, 3 * count_> initialData_;
    State& state_;
public:
    DataBuffer(State& state);
    
    void setMunicipalityData(std::vector<std::pair<int, float>> data);
    void setPostalAreaData(std::vector<std::pair<int, float>> data);
};
