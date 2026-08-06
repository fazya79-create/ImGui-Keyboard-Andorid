#pragma once

#include "Class.hpp"
namespace BNM {
namespace Exceptions {
    struct Exception {
        IL2CPP::Il2CppException *exception{};
        inline operator bool() const { return exception != nullptr; }
        [[nodiscard]] inline std::string ClassName() const { return exception && exception->className ? ((Structures::Mono::String *) exception->className)->str() : std::string{}; }
        [[nodiscard]] inline std::string Message() const { return exception && exception->message ? ((Structures::Mono::String *) exception->message)->str() : std::string{}; }
        [[nodiscard]] inline IL2CPP::Il2CppObject *Data() const { return exception ? exception->_data : nullptr; }
        [[nodiscard]] inline IL2CPP::Il2CppException *GetException() const { return exception; }
        [[nodiscard]] inline bool IsValid() const { return exception != nullptr; }
    };
    Exception TryInvoke(const std::function<void()> &func);
}

namespace Defaults {

namespace Internal {
    extern BNM::Class Void, Boolean, Byte, SByte, Int16, UInt16, Int32, UInt32, IntPtr, UIntPtr, Int64, UInt64, Single, Double, Decimal, String, Object, Type;
    extern BNM::Class Vector2, Vector3, Vector4, Color, Color32, Ray, Quaternion, Matrix3x3, Matrix4x4, RaycastHit, RaycastHit2D;
    extern BNM::Class UnityObject, MonoBehaviour;
}

struct DefaultTypeRef {
    [[nodiscard]] inline bool IsValid() const { return _reference && *_reference; }
    BNM::Class *_reference{};
    inline constexpr DefaultTypeRef() = default;
    inline constexpr DefaultTypeRef(BNM::Class *reference) : _reference(reference) {}
    inline operator BNM::CompileTimeClass() const;
    inline operator BNM::Class() const { return _reference ? *_reference : BNM::Class{}; }
    inline BNM::Class ToClass() const { return _reference ? *_reference : BNM::Class{}; }
};

using byte = uint8_t;
using sbyte = int8_t;
using ushort = uint16_t;

template<typename T>
inline DefaultTypeRef Get();

template<typename T>
inline IL2CPP::Il2CppObject *Box(T value) {
    if constexpr (std::is_pointer_v<T>) return (IL2CPP::Il2CppObject *) value;
    return BNM::Class::BoxObject(BNM::Defaults::Get<T>().ToClass().GetClass(), &value);
}

template<typename T>
inline DefaultTypeRef Get() {
    if constexpr (std::is_same_v<T, void>) return {&Internal::Void};
    else if constexpr (std::is_same_v<T, bool>) return {&Internal::Boolean};
    else if constexpr (std::is_same_v<T, char>) return {&Internal::Byte};
    else if constexpr (std::is_same_v<T, int8_t>) return {&Internal::SByte};
    else if constexpr (std::is_same_v<T, uint8_t>) return {&Internal::Byte};
    else if constexpr (std::is_same_v<T, int16_t>) return {&Internal::Int16};
    else if constexpr (std::is_same_v<T, uint16_t>) return {&Internal::UInt16};
    else if constexpr (std::is_same_v<T, int32_t>) return {&Internal::Int32};
    else if constexpr (std::is_same_v<T, uint32_t>) return {&Internal::UInt32};
    else if constexpr (std::is_same_v<T, int64_t>) return {&Internal::Int64};
    else if constexpr (std::is_same_v<T, uint64_t>) return {&Internal::UInt64};
    else if constexpr (std::is_same_v<T, float>) return {&Internal::Single};
    else if constexpr (std::is_same_v<T, double>) return {&Internal::Double};
    else if constexpr (std::is_same_v<T, Structures::Mono::String *>) return {&Internal::String};
    else if constexpr (std::is_same_v<T, MonoType *>) return {&Internal::Type};
    else if constexpr (std::is_same_v<T, IL2CPP::Il2CppObject *>) return {&Internal::Object};
    else if constexpr (std::is_same_v<T, Structures::Unity::Vector2>) return {&Internal::Vector2};
    else if constexpr (std::is_same_v<T, Structures::Unity::Vector3>) return {&Internal::Vector3};
    else if constexpr (std::is_same_v<T, Structures::Unity::Vector4>) return {&Internal::Vector4};
    else if constexpr (std::is_same_v<T, Structures::Unity::Color>) return {&Internal::Color};
    else if constexpr (std::is_same_v<T, Structures::Unity::Color32>) return {&Internal::Color32};
    else if constexpr (std::is_same_v<T, Structures::Unity::Ray>) return {&Internal::Ray};
    else if constexpr (std::is_same_v<T, Structures::Unity::Quaternion>) return {&Internal::Quaternion};
    else if constexpr (std::is_same_v<T, Structures::Unity::Matrix3x3>) return {&Internal::Matrix3x3};
    else if constexpr (std::is_same_v<T, Structures::Unity::Matrix4x4>) return {&Internal::Matrix4x4};
    else if constexpr (std::is_same_v<T, Structures::Unity::RaycastHit>) return {&Internal::RaycastHit};
    else if constexpr (std::is_same_v<T, Structures::Unity::RaycastHit2D>) return {&Internal::RaycastHit2D};
    else if constexpr (std::is_same_v<T, IL2CPP::Il2CppArray *>) return {&Internal::Object};
    else return {&Internal::Object};
}

}
}
