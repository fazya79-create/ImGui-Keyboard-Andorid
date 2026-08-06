#pragma once

#include "Class.hpp"
namespace BNM {
struct FieldBase {
    IL2CPP::FieldInfo *_data{};
    IL2CPP::Il2CppObject *_instance{};
    uint8_t _isConst : 1 = false;
    uint8_t _isStatic : 1 = false;
    uint8_t _isThreadStatic : 1 = false;
    uint8_t _isInStruct : 1 = false;

    inline constexpr FieldBase() = default;
    inline FieldBase(const FieldBase &other) = default;
    FieldBase(IL2CPP::FieldInfo *info);

    FieldBase &SetInstance(IL2CPP::Il2CppObject *val);
    inline IL2CPP::FieldInfo *GetInfo() const { return _data; }
    inline bool IsValid() const { return _data != nullptr; }
    inline operator bool() const { return IsValid(); }
    void *GetFieldPointer() const;
    inline BNM_PTR GetOffset() const { return _data ? (BNM_PTR) _data->offset - (_isInStruct && !_isStatic && !_isThreadStatic ? sizeof(IL2CPP::Il2CppObject) : 0x0) : 0; }
    BNM::Class GetType() const;
    BNM::Class GetParentClass() const;
    inline std::string str() const {
        if (!_data) return "nullptr";
        std::string result;
        result += Class(_data->parent).str();
        result += ".";
        result += _data->name ? _data->name : "nullptr";
        return result;
    }

    inline FieldBase &operator[](void *instance) { SetInstance((IL2CPP::Il2CppObject *) instance); return *this; }
    inline FieldBase &operator[](IL2CPP::Il2CppObject *instance) { SetInstance(instance); return *this; }

    template<typename NewType>
    inline NewType &cast() const { return (NewType &) *this; }
};

template<typename T>
struct Field : public FieldBase {
    inline constexpr Field() noexcept = default;
    template<typename OtherType>
    inline Field(const Field<OtherType> &other) : FieldBase(other) {}
    inline Field(IL2CPP::FieldInfo *info) : FieldBase(info) {}
    inline Field(const FieldBase &other) : FieldBase(other) {}

    inline T *GetPointer() const {
        auto ptr = GetFieldPointer();
        BNM_LOG_ERR_IF(ptr == nullptr, "Field::GetPointer dead: %s", _data ? str().c_str() : "null");
        return (T *) ptr;
    }

    inline T Get() const {
        BNM_LOG_ERR_IF(!_data, "Field::Get dead field");
        if (!_data) return {};
        if (_isThreadStatic || _isConst) {
            T val{};
            PRIVATE_FieldUtils::GetStaticValue(_data, (void *) &val);
            return val;
        }
        if (auto ptr = GetPointer(); ptr != nullptr) return *ptr;
        return {};
    }

    inline operator T() const { return Get(); }
    inline T operator()() const { return Get(); }

    inline void Set(T value) const {
        BNM_LOG_ERR_IF(!_data, "Field::Set dead field");
        if (!_data) return;
        if (_isConst) {
            BNM_LOG_ERR("Field::Set const field: %s", str().c_str());
            return;
        }
        if (_isThreadStatic) {
            PRIVATE_FieldUtils::SetStaticValue(_data, (void *) &value);
            return;
        }
        if (auto ptr = GetPointer(); ptr != nullptr) *ptr = value;
    }

    inline Field<T> &operator=(T value) { Set(std::move(value)); return *this; }
    inline Field<T> &operator=(const FieldBase &other) { _data = other._data; _instance = other._instance; _isConst = other._isConst; _isStatic = other._isStatic; _isThreadStatic = other._isThreadStatic; _isInStruct = other._isInStruct; return *this; }
};
}
