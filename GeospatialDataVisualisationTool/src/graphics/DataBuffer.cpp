#include "DataBuffer.h"
#include <GL/glew.h>


DataBuffer::DataBuffer(State& state) : state_(state) {
    for (int i=0; i<3*count_; i++) {
        initialData_[i] = 0.65f;
    }
    glGenBuffers(1, &idBuff_);
    glBindBuffer(GL_TEXTURE_BUFFER, idBuff_);
    glBufferData(GL_TEXTURE_BUFFER, 3 * count_ * sizeof(GLfloat), &initialData_, GL_STATIC_DRAW);
    
    glGenTextures(1, &idTex_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_BUFFER, idTex_);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, idBuff_);
}


void DataBuffer::setMunicipalityData(std::vector<std::pair<int, float>> data) {
    std::array<float, 3 * municipalityRange_> newData;
    std::copy(initialData_.begin(), initialData_.begin() + 3 * municipalityRange_ + 1, newData.begin());
    
    for (auto pair : data) {
        float r;
        
        if ((state_.logScale ? std::log(pair.second + 1e-9) > state_.midValLog : pair.second >= state_.midVal)) {
            r = state_.logScale ?
                (std::log(pair.second + 1e-9) - state_.midValLog) / (std::log(state_.maxVal + 1e-9) - state_.midValLog) :
                (pair.second - state_.midVal) / (state_.maxVal - state_.midVal);
            newData[pair.first * 3] = r * state_.colorGradientMax.x + (1-r) * state_.colorGradientMid.x;
            newData[pair.first * 3 + 1] = r * state_.colorGradientMax.y + (1-r) * state_.colorGradientMid.y;
            newData[pair.first * 3 + 2] = r * state_.colorGradientMax.z + (1-r) * state_.colorGradientMid.z;
        } else {
            r = state_.logScale ?
                (state_.midValLog - std::log(pair.second + 1e-9)) / (state_.midValLog - std::log(state_.minVal + 1e-9)) :
                (state_.midVal - pair.second) / (state_.midVal - state_.minVal);
            newData[pair.first * 3] = r * state_.colorGradientMin.x + (1-r) * state_.colorGradientMid.x;
            newData[pair.first * 3 + 1] = r * state_.colorGradientMin.y + (1-r) * state_.colorGradientMid.y;
            newData[pair.first * 3 + 2] = r * state_.colorGradientMin.z + (1-r) * state_.colorGradientMid.z;
        }
    }
    
    glBufferSubData(GL_TEXTURE_BUFFER, 0, 3 * municipalityRange_ * sizeof(GLfloat), &newData);
}


void DataBuffer::setPostalAreaData(std::vector<std::pair<int, float>> data) {
    std::array<float, 3 * (count_ - municipalityRange_)> newData;
    std::copy(initialData_.begin() + 3 * municipalityRange_, initialData_.end(), newData.begin());
    
    for (auto pair : data) {
        continue;
        // todo
    }
    
    glBufferSubData(GL_TEXTURE_BUFFER, ( 3 * municipalityRange_ + 1) * sizeof(GLfloat), 3 * (count_ - municipalityRange_) * sizeof(GLfloat), &newData);
}
