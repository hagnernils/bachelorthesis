//
// Created by Nils Hagner on 22.12.20.
//

#include "Buffer.h"

template<typename T>
BufferView<T>::BufferView() {
    data = dataBegin;
}

template<typename T>
BufferView<T>::BufferView(typename std::vector<T>::const_iterator bufferBegin) {
    dataBegin = bufferBegin;
    data = bufferBegin;
}
