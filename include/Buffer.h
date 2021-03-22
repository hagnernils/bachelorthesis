//
// Created by Nils Hagner on 22.12.20.
//

#ifndef BACHELORTHESIS_BUFFER_H
#define BACHELORTHESIS_BUFFER_H

#include <string>
#include <utility>
#include <vector>

template<typename T>
struct Buffer {
    std::string name;
    std::vector<T> data;

    Buffer(std::string name, const std::vector<T> &data) : name(std::move(name)), data(data) {}
};
typedef Buffer<u_char> ByteBuffer;

template<typename T>
struct BufferView {
    // Iterator that marks beginning of this buffer
    T *const data = nullptr;
    uint32_t byteLength = 0;
    uint16_t byteStride = 0;
    size_t numElements = 0;
    size_t elementSizeInBytes = 0;

    BufferView();

    explicit BufferView(T *bufferBegin);

    const T *begin() { return data; }

    bool isValid() { return data == nullptr; }
};
typedef BufferView<unsigned char> ByteBufferView;


#endif //BACHELORTHESIS_BUFFER_H
