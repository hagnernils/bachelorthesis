//
// Created by Nils Hagner on 22.12.20.
//

#ifndef BACHELORTHESIS_BUFFER_H
#define BACHELORTHESIS_BUFFER_H

#include <string>
#include <vector>

template<typename T>
struct Buffer {
    std::string name;
    std::vector<T> data;
};
typedef Buffer<u_char> ByteBuffer;

template<typename T>
struct BufferView {
    // Iterator that marks beginning of this buffer
    T *const data = nullptr;
    uint32_t byteLength = 0;
    uint16_t byteStride = 1;
    size_t numElements = 0;
    size_t elementSizeInBytes = 0;

    BufferView();

    explicit BufferView(T *bufferBegin);

    const T *begin() { return data; }

    bool isValid() { return data == nullptr; }

    const T &operator[](size_t index) const;
};

#endif //BACHELORTHESIS_BUFFER_H
