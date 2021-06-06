#pragma once

namespace Viking
{
    class Buffer
    {
    public:
        Buffer();
        Buffer(byte* data, uint32_t size);

        static Buffer copy(void* data, size_t size);
        void allocate(size_t size);
        void zeroize() const;
        void reset();

        operator bool() const;

        byte& operator[](uint32_t index);
        byte operator[](uint32_t index) const;

        byte* mData;
        size_t mSize;
    };
}
