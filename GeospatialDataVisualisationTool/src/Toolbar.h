#pragma once
#include "State.h"
#include "DataLoader.h"
#include "RegionMappingLoader.h"


class Toolbar {
    State& state_;
    DataLoader& loader_;
    RegionMappingLoader& mappingLoader_;
    int locCol_ = 0;
    int dataCol_ = 1;
    int prevLocCol_ = 1;
    int prevDataCol_ = 0;
    std::vector<int> currentIndices_;
    std::vector<float> currentData_;
    char loadFilePath_[512] { "examples/municipality_data.csv" };
    char delimiter_[2] { ";" };
    bool headerRow_ = true;
    int skipRows_ = 0;
    
    
public:
    Toolbar(State& state, DataLoader& loader, RegionMappingLoader& mappingLoader);
    void draw();
private:
    float drawTools_();
    void drawDataArea_(float y, std::vector<std::vector<std::string>> data);
    void drawDataLoader_(float y);
};
