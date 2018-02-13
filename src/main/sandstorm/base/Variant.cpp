//
// Created by rocklct on 2/13/18.
//

#include "sandstorm/base/Variant.h"

namespace sandstorm {
    namespace base {

        Variant::Variant() : _type(Type::Invalid) {}

        Variant::Variant(const char *stringValue) : _type(Type::String), _stringValue(stringValue) {
        }

        Variant::~Variant() {}

        Variant::Variant(const Variant &variant) : _type(variant._type) {
            if (_type == Type::Int32) {
                _int32Value = variant._int32Value;
            } else if (_type == Type::Int64) {
                _int64Value = variant._int64Value;
            } else if (_type == Type::UInt32) {
                _uint32Value = variant._uint32Value;
            } else if (_type == Type::UInt64) {
                _uint64Value = variant._uint64Value;
            } else if (_type == Type::Boolean) {
                _boolValue = variant._boolValue;
            } else if (_type == Type::Float) {
                _floatValue = variant._floatValue;
            } else if (_type == Type::String) {
                _stringValue = variant._stringValue;
            }
        }

        const Variant &Variant::operator=(const Variant &variant) {
            _type = variant._type;
            if (_type == Type::Int32) {
                _int32Value = variant._int32Value;
            } else if (_type == Type::Int64) {
                _int64Value = variant._int64Value;
            } else if (_type == Type::UInt32) {
                _uint32Value = variant._uint32Value;
            } else if (_type == Type::UInt64) {
                _uint64Value = variant._uint64Value;
            } else if (_type == Type::Boolean) {
                _boolValue = variant._boolValue;
            } else if (_type == Type::Float) {
                _floatValue = variant._floatValue;
            } else if (_type == Type::String) {
                _stringValue = variant._stringValue;
            }

            return *this;
        }
    }

}