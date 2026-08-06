#include "BNM.hpp"

using namespace BNM;

MethodBase::MethodBase(const IL2CPP::MethodInfo *info) {
    if (!info) return;

    _isStatic = (info->flags & 0x0010) == 0x0010;
    _isVirtual = info->slot != 65535;
    _data = (decltype(_data)) info;
}

MethodBase::MethodBase(const IL2CPP::Il2CppReflectionMethod *reflectionMethod) {
    if (!reflectionMethod || !reflectionMethod->method) return;

    auto info = reflectionMethod->method;

    _isStatic = (info->flags & 0x0010) == 0x0010;
    _isVirtual = info->slot != 65535;
    _data = (decltype(_data)) info;
}

MethodBase &MethodBase::SetInstance(IL2CPP::Il2CppObject *val) {
    if (!_data) return *this;
    if (_isStatic) {
        BNM_LOG_WARN("SetInstance on static method: %s", str().c_str());
        return *this;
    }
    _instance = val;
    return *this;
}

MethodBase MethodBase::GetGeneric(const std::initializer_list<CompileTimeClass> &templateTypes) const {
    if (!_data) return {};
    if (!_data->is_generic) {
        BNM_LOG_WARN("GetGeneric on non-generic method: %s", str().c_str());
        return {};
    }
    return Internal::TryMakeGenericMethod(*this, templateTypes);
}

MethodBase MethodBase::GetOverride() const {
    if (!_data || _isStatic || (_data->flags & 0x0040) == 0) return {};
    if (!Utils::IsAllocated(_instance)) {
        BNM_LOG_WARN("GetOverride dead instance: %s", str().c_str());
        return {};
    }

    auto klass = _instance->klass;
    void *iter = nullptr;
    while (auto method = Internal::api.il2cpp_class_get_methods(klass, &iter)) {
        auto vMethod = (IL2CPP::MethodInfo *) method;
        if (strcmp(vMethod->name, _data->name) != 0 || vMethod->parameters_count != _data->parameters_count) continue;
        bool match = true;
        for (uint8_t p = 0; p < vMethod->parameters_count; ++p) {
            auto type = Internal::api.il2cpp_method_get_param(vMethod, p);
            auto type2 = Internal::api.il2cpp_method_get_param(_data, p);
            if (Class(type).GetClass() != Class(type2).GetClass()) {
                match = false;
                break;
            }
        }
        if (!match) continue;
        if (vMethod->slot != 65535 && vMethod->slot < klass->vtable_count) {
            auto &vTable = klass->vtable[vMethod->slot];
            if (vTable.method) return MethodBase(vTable.method)[_instance];
        }
        return MethodBase(vMethod)[_instance];
    }
    return {};
}

BNM::Class MethodBase::GetReturnType() const {
    if (!_data) return {};
    return Internal::api.il2cpp_method_get_return_type(_data);
}

BNM::Class MethodBase::GetParentClass() const {
    if (!_data) return {};
    return Internal::api.il2cpp_method_get_class(_data);
}

namespace BNM::PRIVATE_FieldUtils {
    void GetStaticValue(IL2CPP::FieldInfo *info, void *value) {
        return Internal::api.il2cpp_field_static_get_value(info, value);
    }

    void SetStaticValue(IL2CPP::FieldInfo *info, void *value) {
        return Internal::api.il2cpp_field_static_set_value(info, value);
    }
}

bool BNM::InvokeHookImpl(IL2CPP::MethodInfo *info, void *newMet, void **oldMet) {
    if (!info) return false;
    if (oldMet) *oldMet = (void *) info->methodPointer;
    info->methodPointer = (IL2CPP::Il2CppMethodPointer) newMet;
    return true;
}

bool BNM::VirtualHookImpl(Class targetClass, IL2CPP::MethodInfo *info, void *newMet, void **oldMet) {
    if (!info || !targetClass) return false;

    auto klass = targetClass._data;
    if (!klass || info->slot >= klass->vtable_count) return false;

    auto &vTable = klass->vtable[info->slot];
    if (vTable.method == nullptr) return false;

    if (oldMet) *oldMet = (void *) vTable.methodPtr;
    vTable.methodPtr = (IL2CPP::Il2CppMethodPointer) newMet;
    return true;
}
