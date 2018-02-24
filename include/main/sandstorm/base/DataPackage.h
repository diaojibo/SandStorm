//
// Created by rocklct on 1/10/18.
//

#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>
#include <map>
#include <memory>
#include <vector>
#include "sandstorm/base/ByteArray.h"
#include "sandstorm/base/Variant.h"

namespace sandstorm {
    namespace base {


        class Writable {
        public:
            virtual std::string GetType() const = 0;

            virtual int32_t Read(ByteArrayReader &reader, Variant &variant) = 0;

            virtual int32_t Write(ByteArrayWriter &writer, const Variant &variant) = 0;
        };

        class Int32Writable : public Writable {
        public:
            std::string GetType() const {
                return "int32";
            }

            int32_t Read(ByteArrayReader &reader, Variant &variant) override;

            int32_t Write(ByteArrayWriter &writer, const Variant &variant) override;
        };

        class Int64Writable : public Writable {
        public:
            std::string GetType() const {
                return "int64";
            }

            int32_t Read(ByteArrayReader &reader, Variant &variant) override;

            int32_t Write(ByteArrayWriter &writer, const Variant &variant) override;
        };

        class UInt32Writable : public Writable {
        public:
            std::string GetType() const {
                return "uint32";
            }

            int32_t Read(ByteArrayReader &reader, Variant &variant) override;

            int32_t Write(ByteArrayWriter &writer, const Variant &variant) override;
        };

        class UInt64Writable : public Writable {
        public:
            std::string GetType() const {
                return "uint64";
            }

            int32_t Read(ByteArrayReader &reader, Variant &variant) override;

            int32_t Write(ByteArrayWriter &writer, const Variant &variant) override;
        };

        class BooleanWritable : public Writable {
        public:
            std::string GetType() const {
                return "boolean";
            }

            int32_t Read(ByteArrayReader &reader, Variant &variant) override;

            int32_t Write(ByteArrayWriter &writer, const Variant &variant) override;
        };

        class FloatWritable : public Writable {
        public:
            std::string GetType() const {
                return "float";
            }

            int32_t Read(ByteArrayReader &reader, Variant &variant) override;

            int32_t Write(ByteArrayWriter &writer, const Variant &variant) override;
        };

        class StringWritable : public Writable {
        public:
            std::string GetType() const {
                return "string";
            }

            int32_t Read(ByteArrayReader &reader, Variant &variant) override;

            int32_t Write(ByteArrayWriter &writer, const Variant &variant) override;
        };

        std::map<int8_t, std::shared_ptr<Writable>> &GetWritables();

        class DataPackage {
        public:
            DataPackage();

            void AddVariant(const Variant &variant);

            ByteArray Serialize();

            void Deserialize(const ByteArray &data);

            const Variants &GetVariants() const {
                return _variants;
            }

        private:
            ByteArray SerializeBody();

            ByteArray SerializeHead(int32_t bodySize);

            void DeserializeHead(ByteArrayReader &reader);

            void DeserializeBody(ByteArrayReader &reader);

            Variant DeserializeVariant(ByteArrayReader &reader);

            void SerializeVariant(ByteArrayWriter &writer, const Variant &variant);

            int8_t _version;
            int32_t _length;
            std::vector<Variant> _variants;
        };

    }
}