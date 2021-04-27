#include "Toolbar.h"
#include "imgui/imgui.h"
#include <limits>


Toolbar::Toolbar(State& state, DataLoader& loader, RegionMappingLoader& mappingLoader) : state_(state), loader_(loader), mappingLoader_(mappingLoader) {
    currentIndices_.reserve(3028);
    currentData_.reserve(3028);
}


void Toolbar::draw() {
    float y = drawTools_();
    
    if (loader_.hasValidData()) {
        auto data = loader_.getRows();
        drawDataArea_(y, data);
        
        if (dataCol_ != prevDataCol_ || locCol_ != prevLocCol_) {
            auto indexMapping = mappingLoader_.getMunicipalityIdMapping((regionDataType)state_.locationDataType);
            currentIndices_.clear();
            currentData_.clear();
            float minVal = std::numeric_limits<float>::max();
            float maxVal = std::numeric_limits<float>::min();
            
            bool firstRow = true;
            for (auto row : data) {
                if (firstRow && loader_.hasHeaderRow()) {
                    firstRow = false;
                    continue;
                }
                float d = std::stof(row[dataCol_]);
                currentIndices_.push_back(indexMapping[row[locCol_]]);
                currentData_.push_back(d);
                if (d < minVal)
                    minVal = d;
                else if (d > maxVal)
                    maxVal = d;
            }
            state_.currentIndices = currentIndices_;
            state_.currentData = currentData_;
            state_.minVal = minVal;
            state_.maxVal = maxVal;
            state_.midVal = (minVal + maxVal) / 2;
            state_.midValLog = (std::log(minVal + 1e-9) + std::log(maxVal + 1e-9)) / 2;
            prevLocCol_ = locCol_;
            prevDataCol_ = dataCol_;
        }
    } else {
        drawDataLoader_(y);
        prevLocCol_ = -1;
        prevDataCol_ = -1;
    }
}


float Toolbar::drawTools_() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(500, 0));
    ImGui::Begin("Tools");
    const char* locationTypes[] {"Country", "Postal areas", "Municipalities", "Regions", "AVI regions", "ELY regions", "Subregions", "NUTS 2 regions", "Municipality groupings", "Language regions"};
    int currentLocationType = state_.locationType;
    ImGui::ListBox("Location type", &currentLocationType, locationTypes, IM_ARRAYSIZE(locationTypes));
    if (currentLocationType == 0) state_.locationType = country;
    else if (currentLocationType == 1) state_.locationType = postalAreas;
    else if (currentLocationType == 2) state_.locationType = municipalities;
    else if (currentLocationType == 3) state_.locationType = regions;
    else if (currentLocationType == 4) state_.locationType = aviRegions;
    else if (currentLocationType == 5) state_.locationType = elyRegions;
    else if (currentLocationType == 6) state_.locationType = subregions;
    else if (currentLocationType == 7) state_.locationType = nuts2Regions;
    else if (currentLocationType == 8) state_.locationType = groupings;
    else if (currentLocationType == 9) state_.locationType = languageRegions;
    
    const char* locationDataTypes[] {"IDs", "Finnish names", "Swedish names", "English names"};
    int currentLocationDataType = state_.locationDataType;
    ImGui::ListBox("Location data type", &currentLocationDataType, locationDataTypes, IM_ARRAYSIZE(locationDataTypes));
    if (currentLocationDataType == 0)
        state_.locationDataType = ids;
    else if (currentLocationDataType == 1)
        state_.locationDataType = finnish;
    else if (currentLocationDataType == 2)
        state_.locationDataType = swedish;
    else if (currentLocationDataType == 3)
        state_.locationDataType = english;
    
    ImGui::Checkbox("Log scale", &state_.logScale);
    
    ImGui::Text("Color gradient ");
    ImGui::SameLine();
    ImGui::ColorEdit3("Min", &state_.colorGradientMin.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::SameLine();
    ImGui::ColorEdit3("Mid", &state_.colorGradientMid.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::SameLine();
    ImGui::ColorEdit3("Max", &state_.colorGradientMax.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    
    float height = ImGui::GetWindowHeight();
    ImGui::End();
    return height;
}


void Toolbar::drawDataArea_(float y, std::vector<std::vector<std::string>> data) {
    ImGui::SetNextWindowPos(ImVec2(0, y));
    ImGui::SetNextWindowSize(ImVec2(500, 0));
    ImGui::Begin("Data");
    
    if (ImGui::Button("Open new file")) {
        loader_.reset();
        state_.currentData.clear();
        state_.currentIndices.clear();
    }
    ImGui::Spacing();
    
    int nCol = (int)(data[0].size());
    ImGui::Columns(nCol, nullptr, true);
    
    ImGui::SetWindowFontScale(0.7f);
    for (int j=0; j<nCol; j++) {
        std::string locButtonName = "loc##" + std::to_string(j);
        std::string dataButtonName = "data##" + std::to_string(j);
        ImGui::RadioButton(locButtonName.c_str(), &locCol_, j);
        ImGui::SameLine();
        ImGui::RadioButton(dataButtonName.c_str(), &dataCol_, j);
        ImGui::NextColumn();
    }
    ImGui::SetWindowFontScale(1.f);
    
    ImGui::Separator();
    
    for (int i=0; i<data.size(); i++) {
        for (int j=0; j<nCol; j++) {
            const char* text = data[i][j].c_str();
            ImGui::Text("%s", text);
            ImGui::NextColumn();
        }
        if (i == 0 && loader_.hasHeaderRow())
            ImGui::Separator();
    }
    
    ImGui::End();
}


void Toolbar::drawDataLoader_(float y) {
    ImGui::SetNextWindowPos(ImVec2(0, y));
    ImGui::SetNextWindowSize(ImVec2(500, 0));
    ImGui::Begin("Data");
    
    ImGui::InputText("File path", loadFilePath_, IM_ARRAYSIZE(loadFilePath_));
    ImGui::InputText("Delimiter", delimiter_, IM_ARRAYSIZE(delimiter_));
    ImGui::InputInt("Number of rows to skip", &skipRows_);
    ImGui::Checkbox("Does the file have a header row?", &headerRow_);
    
    if (ImGui::Button("Load")) {
        loader_.readCSV(loadFilePath_, delimiter_[0], headerRow_, skipRows_);
    }
    
    ImGui::End();
}
