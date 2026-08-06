#pragma once

#include "Class.hpp"
#include "Internals.hpp"
namespace BNM {
struct MethodBase {
    IL2CPP::MethodInfo *_data{};
    IL2CPP::Il2CppObject *_instance{};
    uint8_t _isStatic : 1 = false;
    uint8_t _isVirtual : 1 = false;

    inline constexpr MethodBase() = default;
    inline MethodBase(const MethodBase &other) = default;
    MethodBase(const IL2CPP::MethodInfo *info);
    MethodBase(const IL2CPP::Il2CppReflectionMethod *reflectionMethod);

    MethodBase &SetInstance(IL2CPP::Il2CppObject *val);
    inline IL2CPP::MethodInfo *GetInfo() const { return _data; }
    inline BNM_PTR GetOffset() const { return _data ? (BNM_PTR) _data->methodPointer : 0; }
    inline bool IsValid() const { return _data != nullptr; }
    inline operator bool() const { return IsValid(); }

    MethodBase GetGeneric(const std::initializer_list<CompileTimeClass> &templateTypes) const;
    MethodBase GetOverride() const;
    BNM::Class GetReturnType() const;
    BNM::Class GetParentClass() const;

    inline MethodBase &operator[](void *instance) { SetInstance((IL2CPP::Il2CppObject *) instance); return *this; }
    inline MethodBase &operator[](IL2CPP::Il2CppObject *instance) { SetInstance(instance); return *this; }

    template<typename NewType>
    inline NewType &cast() const { return (NewType &) *this; }

    inline std::string str() const {
        if (!_data) return "nullptr";
        std::string result;
        result += Class(PRIVATE_INTERNAL::GetMethodClass(_data)).str();
        result += ".";
        result += _data->name ? _data->name : "nullptr";
        result += "(";
        result += std::to_string(_data->parameters_count);
        result += ")";
        return result;
    }
};

template<typename Ret>
struct Method : public MethodBase {
    inline constexpr Method() noexcept = default;
    template<typename OtherType>
    inline Method(const Method<OtherType> &other) : MethodBase(other) {}
    inline Method(const IL2CPP::MethodInfo *info) : MethodBase(info) {}
    inline Method(const IL2CPP::Il2CppReflectionMethod *reflectionMethod) : MethodBase(reflectionMethod) {}
    inline Method(const MethodBase &other) : MethodBase(other) {}

    inline Method<Ret> &operator[](void *instance) { SetInstance((IL2CPP::Il2CppObject *) instance); return *this; }
    inline Method<Ret> &operator[](IL2CPP::Il2CppObject *instance) { SetInstance(instance); return *this; }

    template<typename ...Parameters>
    inline Ret Call(Parameters ...parameters) const {
        if (!_data) {
            BNM_LOG_ERR("Method::Call dead method");
            return PRIVATE_INTERNAL::ReturnEmpty<Ret>();
        }
        if (sizeof...(Parameters) != _data->parameters_count) {
            BNM_LOG_WARN("Method::Call param count mismatch: %s", str().c_str());
        }
        if (!_isStatic && !Utils::IsAllocated(_instance)) {
            BNM_LOG_ERR("Method::Call dead instance: %s", str().c_str());
            return PRIVATE_INTERNAL::ReturnEmpty<Ret>();
        }
        auto method = _data;
        if (!_isStatic) {
            return ((Ret(*)(IL2CPP::Il2CppObject *, Parameters...)) method->methodPointer)(_instance, parameters...);
        }
        if (_isStatic) {
            return ((Ret(*)(Parameters...)) method->methodPointer)(parameters...);
        }
        return ((Ret(*)(void *, Parameters...)) method->methodPointer)(nullptr, parameters...);
    }

    template<typename ...Parameters>
    inline Ret operator()(Parameters ...parameters) const { return Call(parameters...); }
    template<typename ...Parameters>
    inline IL2CPP::Il2CppObject *CreateNewObjectTypes(const std::initializer_list<std::string_view> &parameterNames, Parameters ...parameters) const {
        if (!_data) return nullptr;
        Class(_data->klass).TryInit();
        auto method = Class(_data->klass).GetMethod(".ctor", parameterNames);
        auto instance = Class(_data->klass).CreateNewInstance();
        if (!instance) return nullptr;
        method.cast<Method<void>>()[instance](parameters...);
        return instance;
    }

    inline Ret Invoke() const {
        if (!_data) return PRIVATE_INTERNAL::ReturnEmpty<Ret>();
        IL2CPP::Il2CppException *exc = nullptr;
        auto ret = Internal::api.il2cpp_runtime_invoke(_data, _instance, nullptr, &exc);
        if (exc) BNM_LOG_ERR("Method::Invoke exception: %s", exc->message ? ((Structures::Mono::String *) exc->message)->str().c_str() : "unknown");
        if constexpr (!std::is_void_v<Ret>) {
            if constexpr (std::is_pointer_v<Ret>) return (Ret) ret;
            return ret ? *(Ret *) Internal::api.il2cpp_object_unbox((IL2CPP::Il2CppObject *) ret) : Ret{};
        }
    }

    template<typename ...Parameters>
    inline Ret Invoke(Parameters ...parameters) const {
        if (!_data) return PRIVATE_INTERNAL::ReturnEmpty<Ret>();
        IL2CPP::Il2CppObject *boxed[sizeof...(Parameters) > 0 ? sizeof...(Parameters) : 1];
        size_t idx = 0;
        ((boxed[idx] = BoxInvokeArg(_data, idx, parameters), ++idx), ...);
        IL2CPP::Il2CppException *exc = nullptr;
        auto ret = Internal::api.il2cpp_runtime_invoke(_data, _instance, sizeof...(Parameters) > 0 ? (void **) boxed : nullptr, &exc);
        if (exc) BNM_LOG_ERR("Method::Invoke exception: %s", exc->message ? ((Structures::Mono::String *) exc->message)->str().c_str() : "unknown");
        if constexpr (!std::is_void_v<Ret>) {
            if constexpr (std::is_pointer_v<Ret>) return (Ret) ret;
            return ret ? *(Ret *) Internal::api.il2cpp_object_unbox((IL2CPP::Il2CppObject *) ret) : Ret{};
        }
    }

private:
    template<typename T>
    static IL2CPP::Il2CppObject *BoxInvokeArg(IL2CPP::MethodInfo *method, size_t index, T value) {
        if constexpr (std::is_pointer_v<T>) {
            return (IL2CPP::Il2CppObject *) value;
        } else {
            auto type = Internal::api.il2cpp_method_get_param(method, (uint32_t) index);
            auto cls = type ? Internal::api.il2cpp_class_from_il2cpp_type(type) : nullptr;
            if (!cls || !Internal::api.il2cpp_value_box) return nullptr;
            unsigned long long storage = 0;
            memcpy(&storage, &value, sizeof(T));
            return Internal::api.il2cpp_value_box(cls, &storage);
        }
    }
};
}
