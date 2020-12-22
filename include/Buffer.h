//
// Created by Nils Hagner on 22.12.20.
//

#ifndef BACHELORTHESIS_BUFFER_H
#define BACHELORTHESIS_BUFFER_H
#include <string>
#include <vector>

template <typename T>
struct Buffer {
    std::string name;
    std::vector<T> data;
};
typedef Buffer<u_char> ByteBuffer;

template <typename T>
struct BufferView {
    // Iterator that marks beginning of this buffer
    typename std::vector<T>::const_iterator data;
    uint32_t byteOffset;
    uint32_t byteLength;
    uint16_t byteStride;
    size_t numElements;
    size_t elementSizeInBytes;
    BufferView();
    explicit BufferView(typename std::vector<T>::const_iterator bufferBegin);
    typename std::vector<T>::const_iterator begin() { return dataBegin; }
private:
    const typename std::vector<T>::const_iterator dataBegin;
};




#endif //BACHELORTHESIS_BUFFER_H
