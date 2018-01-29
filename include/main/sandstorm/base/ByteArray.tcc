//
// Created by rocklct on 1/28/18.
//

namespace sandstorm{
    namespace base{
        template <class T>
        int32_t ByteArrayReader::Read(T* buffer, int32_t count) {
            if ( _pos >= static_cast<int32_t>(_buffer.size()) ) {
                return 0;
            }

            size_t sizeToRead = sizeof(T) * count;
            if ( _pos + sizeToRead > static_cast<int32_t>(_buffer.size()) ) {
                sizeToRead = _buffer.size() - _pos;
            }

            memcpy(buffer, _pos + _buffer.data(), sizeToRead);
            _pos += static_cast<int32_t>(sizeToRead);

            return static_cast<int32_t>(sizeToRead);
        }

        template <class T>
        T ByteArrayReader::Read() {
            T t;
            Read(&t, 1);

            return t;
        }

        int32_t ByteArrayReader::Tell() const {
            return _pos;
        }

        template <class T>
        int32_t ByteArrayWriter::Write(const T* buffer, int32_t count) {
            int32_t sizeToWrite = sizeof(T) * count;
            ByteArray buffer2((const char*)(buffer), sizeToWrite);
            _buffer.Concat(buffer2);

            return sizeToWrite;
        }

        template <class T>
        int32_t ByteArrayWriter::Write(const T& value) {
            return Write(&value, 1);
        }

        const ByteArray& ByteArrayWriter::ToByteArray() const {
            return _buffer;
        }

        int32_t ByteArrayWriter::Tell() const {
            return static_cast<int32_t>(_buffer.size());
        }
    }
}