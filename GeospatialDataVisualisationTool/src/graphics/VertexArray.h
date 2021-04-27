#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"


class VertexArray {
    unsigned int id_;
    
public:
    VertexArray();
    ~VertexArray();
    
    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, unsigned int offset = 0);
    void bind() const;
    void unbind() const;
};


