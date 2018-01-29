//
// Created by rocklct on 1/28/18.
//

#pragma once

namespace sandstorm {
    namespace base {
        Values::Values(std::initializer_list <Value> values) : std::vector<Value>(values) {
        }

        Tuple::Tuple(std::initializer_list <Value> values) : _values(values) {
        }

        void Tuple::Add(const Value &value) {
            _values.push_back(value);
        }

        void Tuple::Pop() {
            _values.pop_back();
        }
    }
}
