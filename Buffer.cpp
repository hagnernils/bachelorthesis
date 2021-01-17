//
// Created by Nils Hagner on 22.12.20.
//

#include "Buffer.h"

template<typename T>
BufferView<T>::BufferView() = default;

template<typename T>
BufferView<T>::BufferView(T *bufferBegin) : data(bufferBegin) {}