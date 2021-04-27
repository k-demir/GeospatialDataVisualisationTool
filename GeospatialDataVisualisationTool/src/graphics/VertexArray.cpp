#include "VertexArray.h"
#include <GL/glew.h>
#include <iostream>

VertexArray::VertexArray() { 
    glGenVertexArrays(1, &id_);
}


VertexArray::~VertexArray() { 
    glDeleteVertexArrays(1, &id_);
}


void VertexArray::addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout, unsigned int offset) {
    bind();
    vb.bind();
    const auto& elements = layout.getElements();
    unsigned int element_offset = 0;
    for (unsigned int i=0; i<elements.size(); i++) {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (void*)(offset + element_offset));
        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }
}

void VertexArray::bind() const { 
    glBindVertexArray(id_);
}


void VertexArray::unbind() const { 
    glBindVertexArray(0);
}
