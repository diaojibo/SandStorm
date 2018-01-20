//
// Created by rocklct on 1/18/18.
//

#ifndef SANDSTORM_DATASINK_H
#define SANDSTORM_DATASINK_H

#include "Net.h"

#include <cstdint>
#include <functional>

namespace sandstorm {
    namespace network {
        class DataSink {
        public:
            virtual int32_t Write(IStream *stream, const char *buf, int64_t bytes) = 0;

            IStream::DataHandler StreamDataHandler(IStream *stream) {
                return std::bind(&DataSink::Write, this, stream, std::placeholders::_1, std::placeholders::_2);
            }
        };
    }
}


#endif //SANDSTORM_DATASINK_H
