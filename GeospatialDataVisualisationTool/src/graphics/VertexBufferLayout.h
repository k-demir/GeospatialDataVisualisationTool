#pragma once

#include <vector>
#include <GL/glew.h>


struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;
    
    static unsigned int getSizeOfType(unsigned int type) {
        switch (type) {
            case GL_FLOAT:          return 4;
            case GL_UNSIGNED_INT:   return 4;
            case GL_UNSIGNED_SHORT:  return 2;
        }
        return 0;
    }
};


template<typename T>
bool runtimeFalse() {
    return false;
}


class VertexBufferLayout {
    std::vector<VertexBufferElement> elements_;
    unsigned int stride_;
    
public:
    VertexBufferLayout() : stride_(0) {};
    
    template <typename T>
    void push(unsigned int count) {
        static_assert(runtimeFalse<T>(), "");
    }
    
    template <>
    void push<float>(unsigned int count) {
        elements_.push_back({GL_FLOAT, count, GL_FALSE});
        stride_ += VertexBufferElement::getSizeOfType(GL_FLOAT) * count;
    }
    
    template <>
    void push<unsigned int>(unsigned int count) {
        elements_.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
        stride_ += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count;
    }
    
    template <>
    void push<unsigned char>(unsigned int count) {
        elements_.push_back({GL_UNSIGNED_SHORT, count, GL_FALSE});
        stride_ += VertexBufferElement::getSizeOfType(GL_UNSIGNED_SHORT) * count;
    }
    
    inline const std::vector<VertexBufferElement>& getElements() const { return elements_; };
    inline unsigned int getStride() const { return stride_; };
};
