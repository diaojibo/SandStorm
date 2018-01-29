//
// Created by rocklct on 1/27/18.
//

#include "sandstorm/base/Values.h"
#include "sandstorm/base/Variant.h"

namespace sandstorm {
    namespace base {
        Variant Value::ToVariant() const {
            if (_type == Type::Int8) {
                return Variant(static_cast<int32_t>(ToInt8()));
            } else if (_type == Type::Int16) {
                return Variant(static_cast<int32_t>(ToInt16()));
            } else if (_type == Type::Int32) {
                return Variant(ToInt32());
            } else if (_type == Type::String) {
                return Variant(ToString());
            }

            return Variant();
        }

        Value Value::FromVariant(const Variant &variant) {
            if (variant.GetType() == Variant::Type::Integer) {
                return Value(variant.GetIntValue());
            } else if (variant.GetType() == Variant::Type::String) {
                return Value(variant.GetStringValue());
            }

            return Value();
        }
    }
}