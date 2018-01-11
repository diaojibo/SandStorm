//
// Created by rocklct on 1/10/18.
//

#include "sandstorm/base/DataPackage.h"

namespace sandstorm {
    namespace base {
        std::map<int8_t, std::shared_ptr<Writable>> Writables =
                {
                        {0, std::shared_ptr<Writable>(new IntWritable)},
                        {3, std::shared_ptr<Writable>(new StringWritable)}
                };

        std::map<Variant::Type, int8_t> Variant::TypeCodes = {
                {Variant::Type::Integer, 0},
                {Variant::Type::String,  3}
        };

        std::map<Variant::Type, std::string> Variant::TypeName = {
                {Variant::Type::Invalid, "Invalid"},
                {Variant::Type::Integer, "Integer"},
                {Variant::Type::Boolean, "Boolean"},
                {Variant::Type::Float,   "Float"},
                {Variant::Type::String,  "String"}
        };
    }
}
