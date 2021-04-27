#include "Renderer.h"
#include <array>
#include <algorithm>
#include <utility>


Renderer::Renderer(State& state, RegionMappingLoader& mappingLoader)
: state_(state), mappingLoader_(mappingLoader), vb_((state.vertex_count+state.municipality_vertex_count)*3*sizeof(float)), ib_(state.index_count + state.municipality_index_count), db_(state), displayAreasShader_("res/shaders/DisplayAreas.shader"), displayEmptyMapShader_("res/shaders/DisplayEmptyMap.shader"), displayAggregationShader_("res/shaders/DisplayAggregation.shader"), displayAreasWithDataShader_("res/shaders/DisplayAreasWithData.shader") {
    
    initializeBuffers_();
    proj_ = glm::ortho(state.min_x, state.max_x, state.min_y, state.max_y, -1.0f, 1.0f);
    view_ = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
}


void Renderer::draw() {
    std::vector<std::pair<int, float>> dataPairs;
    
    switch (state_.locationType) {
        case country:
            initializeMVPmatrix_(displayEmptyMapShader_);
            displayEmptyMapShader_.setUniformVec4f("u_fgColor", state_.mapColor);
            drawElements_(vaPostalAreas_, ib_, displayEmptyMapShader_);
            break;
        case postalAreas:
            initializeMVPmatrix_(displayAreasShader_);
            drawElements_(vaPostalAreas_, ib_, displayAreasShader_);
            break;
        case municipalities:
            if (state_.currentData.empty()) {
                initializeMVPmatrix_(displayAreasShader_);
                drawElements_(vaMunicipalities_, ib_, displayAreasShader_);
            } else {
                initializeMVPmatrix_(displayAreasWithDataShader_);
                for (int i=0; i<state_.currentIndices.size(); i++) {
                    dataPairs.push_back(std::pair<int, float> { state_.currentIndices[i], state_.currentData[i] });
                }
                db_.setMunicipalityData(dataPairs);
                displayAreasWithDataShader_.setUniform1i("sb", 0);
                drawElements_(vaMunicipalities_, ib_, displayAreasWithDataShader_);
            }
            break;
        case regions:
            initializeMVPmatrix_(displayAggregationShader_);
            displayAggregationShader_.setUniform1iv("u_idMap", 1000, &(regionIds_[0]));
            drawElements_(vaMunicipalities_, ib_, displayAggregationShader_);
            break;
        case aviRegions:
            initializeMVPmatrix_(displayAggregationShader_);
            displayAggregationShader_.setUniform1iv("u_idMap", 1000, &(aviIds_[0]));
            drawElements_(vaMunicipalities_, ib_, displayAggregationShader_);
            break;
        case elyRegions:
            initializeMVPmatrix_(displayAggregationShader_);
            displayAggregationShader_.setUniform1iv("u_idMap", 1000, &(elyIds_[0]));
            drawElements_(vaMunicipalities_, ib_, displayAggregationShader_);
            break;
        case subregions:
            initializeMVPmatrix_(displayAggregationShader_);
            displayAggregationShader_.setUniform1iv("u_idMap", 1000, &(subregionIds_[0]));
            drawElements_(vaMunicipalities_, ib_, displayAggregationShader_);
            break;
        case nuts2Regions:
            initializeMVPmatrix_(displayAggregationShader_);
            displayAggregationShader_.setUniform1iv("u_idMap", 1000, &(nuts2Ids_[0]));
            drawElements_(vaMunicipalities_, ib_, displayAggregationShader_);
            break;
        case groupings:
            initializeMVPmatrix_(displayAggregationShader_);
            displayAggregationShader_.setUniform1iv("u_idMap", 1000, &(groupingIds_[0]));
            drawElements_(vaMunicipalities_, ib_, displayAggregationShader_);
            break;
        case languageRegions:
            initializeMVPmatrix_(displayAggregationShader_);
            displayAggregationShader_.setUniform1iv("u_idMap", 1000, &(languageRegionIds_[0]));
            drawElements_(vaMunicipalities_, ib_, displayAggregationShader_);
            break;
    }
}


void Renderer::clear() const {
    glClearColor(state_.clearColor[0], state_.clearColor[1], state_.clearColor[2], state_.clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}


void Renderer::drawElements_(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) {
    shader.bind();
    va.bind();
    ib.bind();
    glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, (void*)8);
}


void Renderer::initializeMVPmatrix_(Shader& shader) {
    shader.bind();
    proj_ = glm::ortho(state_.midX-state_.dx/2/state_.zoom, state_.midX+state_.dx/2/state_.zoom, state_.midY-state_.dy/2/state_.zoom, state_.midY+state_.dy/2/state_.zoom, -1.0f, 1.0f);
    model_ = glm::translate(glm::mat4(1.0f), state_.translation);
    mvp_ = proj_ * view_ * model_;
    shader.setUniformMat4f("u_MVP", mvp_);
}


void Renderer::initializeBuffers_() {
    postalAreaVertices_ = GraphicsLoader::loadVertices("res/data/postal_area_vertices", 2 * state_.vertex_count);
    postalAreaIndices_ = GraphicsLoader::loadIndices("res/data/postal_area_indices", state_.index_count);
    municipalityVertices_ = GraphicsLoader::loadVertices("res/data/municipality_vertices", 2 * state_.municipality_vertex_count);
    municipalityIndices_ = GraphicsLoader::loadIndices("res/data/municipality_indices", state_.municipality_index_count);
    regionIds_ = mappingLoader_.getRegionIdArray(region_id);
    aviIds_ = mappingLoader_.getRegionIdArray(avi_id);
    elyIds_ = mappingLoader_.getRegionIdArray(ely_id);
    subregionIds_ = mappingLoader_.getRegionIdArray(subregion_id);
    nuts2Ids_ = mappingLoader_.getRegionIdArray(nuts2_id);
    groupingIds_ = mappingLoader_.getRegionIdArray(grouping_id);
    languageRegionIds_ = mappingLoader_.getRegionIdArray(languages_id);
    
    vb_.addData(&postalAreaVertices_[0], state_.vertex_count*3*sizeof(float));
    vb_.addData(&municipalityVertices_[0], state_.municipality_vertex_count*3*sizeof(float));
    
    layout_.push<float>(2);
    layout_.push<float>(1);
    vaPostalAreas_.addBuffer(vb_, layout_, 0);
    vaMunicipalities_.addBuffer(vb_, layout_, state_.vertex_count*3*sizeof(float));
    
    ib_.addData(&postalAreaIndices_[0], state_.index_count);
    ib_.addData(&municipalityIndices_[0], state_.municipality_index_count);
    
    
}
