#pragma once


class IndexBuffer {
    unsigned int id_;
    unsigned int count_;
    unsigned int currentCount_ = 0;
    
public:
    IndexBuffer(unsigned int count);
    ~IndexBuffer();
    
    void addData(const unsigned int* data, unsigned int count);
    void bind() const;
    void unbind() const;
    inline unsigned int getCount() const { return count_; };
};
