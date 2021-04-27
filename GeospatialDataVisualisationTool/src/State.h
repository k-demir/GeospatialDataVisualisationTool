#pragma once
#include "glm/glm.hpp"
#include <vector>


enum LocationType { country, postalAreas, municipalities, regions, aviRegions, elyRegions, subregions, nuts2Regions, groupings, languageRegions };
enum LocationDataType { finnish = 1, swedish = 2, english = 3, ids = 0 };
enum DrawingMode { displayUnique };


struct State {
    float zoom = 0.95f;
    glm::vec3 translation {0, 0, 0};
    
    bool mousePressed = false;
    double mouseDownX = 0.0;
    double mouseDownY = 0.0;
    glm::vec2 mouseDownTranslation {0, 0};
    
    const float minZoom = 0.5f;
    const float maxZoom = 20.0f;
    const float minTranslationX = -450000.0f;
    const float maxTranslationX = 450000.0f;
    const float minTranslationY = -600000.0f;
    const float maxTranslationY = 600000.0f;
    
    #include "postal_area_params.cpp"
    #include "municipality_params.cpp"
    
    const float dy = max_y - min_y;
    const float dx = max_x - min_x;
    const float midY = min_y + dy/2;
    const float midX = min_x + dx/2;
    
    LocationType locationType = country;
    LocationDataType locationDataType = finnish;
    DrawingMode drawingMode = displayUnique;
    
    glm::vec4 clearColor {1.0f, 1.0f, 1.0f, 1.0f};
    glm::vec4 mapColor {0.2f, 0.4f, 0.12f, 1.0f};
    
    std::vector<int> currentIndices;
    std::vector<float> currentData;
    float minVal = 0;
    float maxVal = 0;
    float midVal = 0;
    float midValLog = 0;
    bool logScale = false;
    
    glm::vec3 colorGradientMin {0.1f, 0.1f, 0.8f};
    glm::vec3 colorGradientMax {0.8f, 0.1f, 0.1f};
    glm::vec3 colorGradientMid {0.8f, 0.8f, 0.8f};
};
