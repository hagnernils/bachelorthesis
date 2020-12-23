//
// Created by Nils Hagner on 22.12.20.
//

#include "Buffer.h"

template<typename T>
BufferView<T>::BufferView() {
}

template<typename T>
BufferView<T>::BufferView(T *bufferBegin) : data(bufferBegin) {}

template<typename T>
const T &BufferView<T>::operator[](size_t index) const {
    return *(data + index * (byteStride ? byteStride : sizeof(T)));
}
