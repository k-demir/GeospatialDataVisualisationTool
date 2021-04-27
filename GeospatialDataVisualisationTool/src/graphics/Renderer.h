#pragma once

#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "DataBuffer.h"
#include "Shader.h"
#include "State.h"
#include "RegionMappingLoader.h"
#include "GraphicsLoader.h"
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


class Renderer {
    State& state_;
    RegionMappingLoader& mappingLoader_;
    
    glm::mat4 proj_;
    glm::mat4 view_;
    glm::mat4 model_;
    glm::mat4 mvp_;
    VertexBuffer vb_;
    VertexBufferLayout layout_;
    IndexBuffer ib_;
    DataBuffer db_;
    
    std::vector<float> postalAreaVertices_;
    std::vector<unsigned int> postalAreaIndices_;
    std::vector<float> municipalityVertices_;
    std::vector<unsigned int> municipalityIndices_;
    std::array<int, 1000> regionIds_;
    std::array<int, 1000> aviIds_;
    std::array<int, 1000> elyIds_;
    std::array<int, 1000> subregionIds_;
    std::array<int, 1000> nuts2Ids_;
    std::array<int, 1000> groupingIds_;
    std::array<int, 1000> languageRegionIds_;
    
    VertexArray vaPostalAreas_;
    VertexArray vaMunicipalities_;
    
    Shader displayAreasShader_;
    Shader displayEmptyMapShader_;
    Shader displayAggregationShader_;
    Shader displayAreasWithDataShader_;
    
public:
    Renderer(State& state, RegionMappingLoader& mappingLoader);
    void draw();
    void clear() const;
private:
    void drawElements_(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
    void initializeMVPmatrix_(Shader& shader);
    void initializeBuffers_();
};
