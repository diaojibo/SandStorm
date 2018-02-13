//
// Created by rocklct on 2/13/18.
//

#include "sandstorm/base/ByteArray.h"

namespace sandstorm {
    namespace base {
        ByteArray::ByteArray(size_t size) :
                std::vector<char>(size) {
        }

        ByteArray::ByteArray(const char *buffer, int32_t size) :
                std::vector<char>(buffer, buffer + size) {
        }

        std::string ByteArray::ToStdString() const {
            std::string result(this->cbegin(), this->cend());

            return result;
        }

        ByteArray &ByteArray::Concat(const ByteArray &buffer2) {
            size_t oldSize = size();
            size_t newSize = oldSize + buffer2.size();
            resize(newSize);
            memcpy(this->data() + oldSize, buffer2.data(), buffer2.size());

            return *this;
        }

        ByteArray ByteArray::operator+(const ByteArray &buffer2) const {
            ByteArray buffer1(this->size() + buffer2.size());
            memcpy(buffer1.data(), this->data(), this->size());
            memcpy(buffer1.data() + this->size(), buffer2.data(), buffer2.size());

            return buffer1;
        }

        ByteArrayReader::ByteArrayReader(const ByteArray &buffer) :
                _buffer(buffer), _pos(0) {}

        ByteArray ByteArrayReader::ReadData(int32_t size) {
            if (_pos >= static_cast<int32_t>(_buffer.size())) {
                return 0;
            }

            size_t sizeToRead = size;
            if (_pos + sizeToRead > static_cast<int32_t>(_buffer.size())) {
                sizeToRead = _buffer.size() - static_cast<size_t>(_pos);
            }

            ByteArray result(sizeToRead);
            memcpy(result.data(), _buffer.data() + _pos, sizeToRead);
            _pos += static_cast<int32_t>(sizeToRead);

            return result;
        }

        void ByteArrayReader::Seek(SeekMode mode, int32_t size) {
            int32_t dest = _pos;
            if (mode == SeekMode::Set) {
                dest = size;
            } else if (mode == SeekMode::Forward) {
                dest += size;
            } else if (mode == SeekMode::Backward) {
                dest -= size;
            }
        }

        int32_t ByteArrayWriter::Write(const ByteArray &buffer) {
            _buffer.Concat(buffer);

            return static_cast<int32_t>(buffer.size());
        }


    }
}