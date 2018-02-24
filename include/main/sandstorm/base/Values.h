//
// Created by Rocklct on 2017/12/24.
//


#pragma once

#include <string>
#include <exception>
#include <cstdint>
#include <vector>
#include <initializer_list>
#include <iostream>
#include "sandstorm/base/Variant.h"

namespace sandstorm {
    namespace base {
        typedef Variant Value;

        class Values : public std::vector<Value> {
        public:
            Values() = default;

            inline Values(std::initializer_list<Value> values);

            Value &operator[](size_t index) {
                return std::vector<Value>::operator[](index);
            }

            const Value &operator[](size_t index) const {
                return std::vector<Value>::operator[](index);
            }
        };

        class Tuple : public base::Serializable {
        public:
            Tuple() = default;

            inline Tuple(std::initializer_list<Value> values);

            Value &operator[](size_t index) {
                return _values[index];
            }

            const Value &operator[](size_t index) const {
                return _values[index];
            }

            Value &operator[](const std::string &fieldName) {
                return _values[_fieldsMap->at(fieldName)];
            }

            const Value &operator[](const std::string &fieldName) const {
                return _values[_fieldsMap->at(fieldName)];
            }

            inline void Add(const Value &value);

            inline void Pop();

            size_t GetSize() const {
                return _values.size();
            }

            void SetSourceTask(const std::string &sourceTask) {
                _sourceTask = sourceTask;
            }

            const std::string &GetSourceTask() const {
                return _sourceTask;
            }

            void SetDestTask(const std::string &destTask) {
                _destTask = destTask;
            }

            const std::string &GetDestTask() const {
                return _destTask;
            }

            const Values &GetValues() const {
                return _values;
            }

            void SetFields(const std::vector<std::string> *fields) {
                _fields = fields;
            }

            const std::vector<std::string> *GetFields() const {
                return _fields;
            }

            void SetFieldsMap(const std::map<std::string, int32_t> *fieldsMap) {
                _fieldsMap = fieldsMap;
            }

            void Serialize(Variants &variants) const override;

            void Deserialize(Variants::const_iterator &it) override;

        private:
            std::string _sourceTask;
            std::string _destTask;
            Values _values;
            const std::vector<std::string> *_fields;
            const std::map<std::string, int32_t> *_fieldsMap;
        };
    }
}

#include "sandstorm/base/Values.tcc"