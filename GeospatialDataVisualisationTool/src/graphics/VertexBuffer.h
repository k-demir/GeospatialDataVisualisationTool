#pragma once


class VertexBuffer {
    unsigned int id_;
    unsigned int currentSize = 0;
    
public:
    VertexBuffer(unsigned int size);
    ~VertexBuffer();
    
    void addData(const void* data, unsigned int size);
    void bind() const;
    void unbind() const;
};
