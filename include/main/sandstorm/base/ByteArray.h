//
// Created by rocklct on 1/9/18.
//

#pragma once

#include <cstring>
#include <vector>
#include <cstdint>

namespace sandstorm {
    namespace base {
        class ByteArray : public std::vector<char> {
        public:
            ByteArray() = default;

            ByteArray(size_t size);

            ByteArray(const char *buffer, int32_t size);

            std::string ToStdString() const;

            ByteArray &Concat(const ByteArray &buffer2);

            ByteArray operator+(const ByteArray &buffer2) const;
        };

        class IODevice {
        public:
            enum class SeekMode {
                Set,
                Forward,
                Backward
            };

            ~IODevice() {}
        };

        class ByteArrayReader : public IODevice {
        public:
            ByteArrayReader(const ByteArray &buffer);

            template<class T>
            int32_t Read(T *buffer, int32_t count);

            template<class T>
            T Read();

            ByteArray ReadData(int32_t size);

            inline int32_t Tell() const;

            void Seek(SeekMode mode, int32_t size);

        private:
            ByteArray _buffer;
            int32_t _pos;
        };

        class ByteArrayWriter {
        public:
            ByteArrayWriter() = default;

            template<class T>
            int32_t Write(const T *buffer, int32_t count);

            template<class T>
            int32_t Write(const T &value);

            int32_t Write(const ByteArray &buffer);

            inline const ByteArray &ToByteArray() const;

            inline int32_t Tell() const;

        private:
            ByteArray _buffer;
        };

    }
}
