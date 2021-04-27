#include "IndexBuffer.h"
#include "Renderer.h"


IndexBuffer::IndexBuffer(unsigned int count) : count_(count) {
    glGenBuffers(1, &id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count_ * sizeof(unsigned int), 0, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &id_);
}


void IndexBuffer::addData(const unsigned int* data, unsigned int count) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, currentCount_, count_ * sizeof(unsigned int), data);
    currentCount_ += count;
}


void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
}


void IndexBuffer::unbind() const { 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
