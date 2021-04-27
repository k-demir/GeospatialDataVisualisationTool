#include "VertexBuffer.h"
#include <GL/glew.h>


VertexBuffer::VertexBuffer(unsigned int size) { 
    glGenBuffers(1, &id_);
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);
}


VertexBuffer::~VertexBuffer() { 
    glDeleteBuffers(1, &id_);
}


void VertexBuffer::addData(const void* data, unsigned int size) {
    glBufferSubData(GL_ARRAY_BUFFER, currentSize, size, data);
    currentSize += size;
}


void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, id_);
}


void VertexBuffer::unbind() const { 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
