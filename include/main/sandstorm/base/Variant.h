//
// Created by Rocklct on 2017/12/31.
//

#pragma once


#include <iostream>
#include <string>
#include <cstdint>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <list>

#define VARIANT_AECCESS(TypeName, CType, valueName) \
    Variant(CType value) : _type(Type::TypeName), valueName(value) {} \
    \
    CType Get##TypeName##Value() const { \
        if ( _type == Type::Invalid ) { \
            LOG(LOG_ERROR) << "Invalid"; \
        } \
    \
        if ( _type == Type::TypeName ) { \
            return valueName; \
        } \
    \
        LOG(LOG_ERROR) << "Type mismatched. " << \
                 "Expected: " << TypeNames[Type::TypeName] << \
                 ". Actually: " << TypeNames[_type]; \
        throw "Type mismatched"; \
    } \
    \
    void Set##TypeName##Value(CType value) { \
        _type = Type::TypeName; \
        valueName = value; \
    } \
    \
    static void Deserialize(Variants::const_iterator& it, CType& value) { \
        value = it->Get##TypeName##Value(); \
        it ++; \
    } \
    \
    static void Serialize(Variants& variants, CType value) { \
        variants.push_back(Variant(value)); \
    }


namespace sandstorm {
    namespace base {
        class Variant;

        typedef std::vector<Variant> Variants;

        class Serializable {
        public:
            virtual void Serialize(Variants &variants) const = 0;

            virtual void Deserialize(Variants::const_iterator &it) = 0;
        };

        class Variant {
        public:
            enum class Type {
                Invalid,
                Int32,
                Int64,
                UInt32,
                UInt64,
                Boolean,
                Float,
                String,
            };

            static std::map<Type, int8_t> TypeCodes;
            static std::map<Type, std::string> TypeNames;

            Variant();

            Variant(const char *stringValue);

            Variant(const Variant &variant);

            ~Variant();

            const Variant &operator=(const Variant &variant);

            Type GetType() const {
                return _type;
            }

            template<class Value>
            Value GetValue() const {
                throw "Not Implemented";
            }

            //Deserialize:convert variant into value
            static inline void Deserialize(Variants::const_iterator &it, Variant &value);

            static inline void Deserialize(Variants::const_iterator &it, Serializable &value);

            template<class Element>
            static void Deserialize(Variants::const_iterator &it, std::vector<Element> &values);

            template<class Element>
            static void Deserialize(Variants::const_iterator &it, std::list<Element> &values);

            template<class Element>
            static void Deserialize(Variants::const_iterator &it, std::set<Element> &values);

            template<class Key, class Element>
            static void Deserialize(Variants::const_iterator &it, std::map<Key, Element> &values);

            static inline void Serialize(Variants &variants, const Variant &value);

            static inline void Serialize(Variants &variants, const Serializable &object);


            //Serialize:Convert value into variant
            template<class Element>
            static void Serialize(Variants &variants, std::vector<Element> values);

            template<class Element>
            static void Serialize(Variants &variants, std::list<Element> values);

            template<class Element>
            static void Serialize(Variants &variants, std::set<Element> values);

            template<class Key, class Element>
            static void Serialize(Variants &variants, std::map<Key, Element> values);

            VARIANT_AECCESS(Int32, int32_t, _int32Value)

            VARIANT_AECCESS(UInt32, uint32_t, _uint32Value)

            VARIANT_AECCESS(Int64, int64_t, _int64Value)

            VARIANT_AECCESS(UInt64, uint64_t, _uint64Value)

            VARIANT_AECCESS(Boolean, bool, _boolValue)

            VARIANT_AECCESS(Float, float, _floatValue)

            VARIANT_AECCESS(String, std::string, _stringValue)

        private:
            Type _type;
            union {
                int32_t _int32Value;
                int64_t _int64Value;
                uint32_t _uint32Value;
                uint64_t _uint64Value;
                bool _boolValue;
                float _floatValue;
            };
            std::string _stringValue;
        };
    }
}

#include "sandstorm/base/Variant.tcc"