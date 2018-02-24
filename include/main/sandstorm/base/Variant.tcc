//
// Created by rocklct on 1/28/18.
//

#pragma once

namespace sandstorm {
    namespace base {
        void Variant::Deserialize(Variants::const_iterator &it, Variant &value) {
            value = *it;
            it++;
        }

        void Variant::Deserialize(Variants::const_iterator &it, Serializable &value) {
            value.Deserialize(it);
        }

        template<class Element>
        void Variant::Deserialize(Variants::const_iterator &it, std::vector<Element> &values) {
            size_t size = 0;

            //get the size of the Variants
            Deserialize(it, size);

            values.resize(size);
            for (Element &value : values) {
                Deserialize(it, value);
            }
        }

        template<class Element>
        void Variant::Deserialize(Variants::const_iterator &it, std::list<Element> &values) {
            size_t size = 0;
            Deserialize(it, size);

            values.resize(size);
            for (Element &value : values) {
                Deserialize(it, value);
            }
        }

        template<class Element>
        void Variant::Deserialize(Variants::const_iterator &it, std::set<Element> &values) {
            size_t size = 0;
            Deserialize(it, size);

            values.clear();
            for (size_t i = 0; i != size; ++i) {
                Element value;
                Deserialize(it, value);

                values.insert(value);
            }
        }

        template<class Key, class Element>
        void Variant::Deserialize(Variants::const_iterator &it, std::map<Key, Element> &values) {
            size_t size = 0;
            Deserialize(it, size);

            for (size_t i = 0; i != size; ++i) {
                Key key;
                Deserialize(it, key);

                Element value;
                Deserialize(it, value);

                values.insert({key, value});
            }
        }

        void Variant::Serialize(Variants &variants, const Variant &value) {
            variants.push_back(value);
        }

        template<class Element>
        void Variant::Serialize(Variants &variants, std::vector<Element> values) {
            variants.push_back(Variant(values.size()));
            for (const Element &value : values) {
                Serialize(variants, value);
            }
        }

        template<class Element>
        void Variant::Serialize(Variants &variants, std::list<Element> values) {
            variants.push_back(Variant(values.size()));
            for (const Element &value : values) {
                Serialize(variants, value);
            }
        }

        template<class Element>
        void Variant::Serialize(Variants &variants, std::set<Element> values) {
            variants.push_back(Variant(values.size()));
            for (const Element &value : values) {
                Serialize(variants, value);
            }
        }

        template<class Key, class Element>
        void Variant::Serialize(Variants &variants, std::map<Key, Element> values) {
            variants.push_back(Variant(values.size()));
            for (const std::pair<Key, Element> &value : values) {
                Serialize(variants, value.first);
                Serialize(variants, value.second);
            }
        }

        void Variant::Serialize(Variants &variants, const Serializable &object) {
            object.Serialize(variants);
        }

        template<>
        inline int32_t Variant::GetValue<int32_t>() const {
            return GetInt32Value();
        }

        template<>
        inline int64_t Variant::GetValue<int64_t>() const {
            return GetInt64Value();
        }

        template<>
        inline uint32_t Variant::GetValue<uint32_t>() const {
            return GetUInt32Value();
        }

        template<>
        inline uint64_t Variant::GetValue<uint64_t>() const {
            return GetUInt64Value();
        }

        template<>
        inline bool Variant::GetValue<bool>() const {
            return GetBooleanValue();
        }

        template<>
        inline float Variant::GetValue<float>() const {
            return GetFloatValue();
        }

        template<>
        inline std::string Variant::GetValue<std::string>() const {
            return GetStringValue();
        }
    }
}