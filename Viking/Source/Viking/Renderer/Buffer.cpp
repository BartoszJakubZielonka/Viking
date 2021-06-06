#include "vipch.h"
#include "Viking/Renderer/Buffer.h"

namespace Viking
{
    Buffer::Buffer() : mData(nullptr), mSize(0)
    {
    }

    Buffer::Buffer(byte* data, uint32_t size):mData(data), mSize(size)
    {
    }

    Buffer::operator bool() const
    {
        return mData;
    }

    byte& Buffer::operator[](uint32_t index)
    {
        return mData[index];
    }

    byte Buffer::operator[](uint32_t index) const
    {
        return mData[index];
    }

    Buffer Buffer::copy(void* data, size_t size)
    {
        Buffer buffer;
        buffer.allocate(size);
        memcpy(buffer.mData, data, size);
        return buffer;
    }

    void Buffer::allocate(const size_t size)
    {
        delete[] mData;
        mData = nullptr;
        mSize = 0;

        mData = new uint8_t[size];
        mSize = size;
    }

    void Buffer::zeroize() const
    {
        if (mData)
            memset(mData, 0, mSize);
    }

    void Buffer::reset()
    {
        delete[] mData;
        mData = nullptr;
        mSize = 0;
    }
}
