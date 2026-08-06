#include "BNM.hpp"

using namespace BNM;

FieldBase::FieldBase(IL2CPP::FieldInfo *info) {
    if (!info) return;

    _isConst = Internal::api.il2cpp_field_is_literal(info);
    auto flags = Internal::api.il2cpp_field_get_flags(info);
    auto offset = (int32_t) Internal::api.il2cpp_field_get_offset(info);
    _isStatic = !_isConst && (flags & 0x0010) != 0 && offset != -1;
    _data = info;
    _isThreadStatic = offset == -1;
    auto parent = Internal::api.il2cpp_field_get_parent(info);
    _isInStruct = parent && Internal::api.il2cpp_class_is_valuetype(parent);
}

FieldBase &FieldBase::SetInstance(IL2CPP::Il2CppObject *val) {
    if (!_data) {
        BNM_LOG_ERR("Field::SetInstance dead field");
        return *this;
    }
    if (_isStatic || _isConst) {
        BNM_LOG_WARN("Field::SetInstance on static/const: %s", str().c_str());
        return *this;
    }
    _instance = val;
    return *this;
}

void *FieldBase::GetFieldPointer() const {
    auto &api = Internal::api;
    if (!_data) return nullptr;
    if (!_isStatic && !Utils::IsAllocated(_instance)) {
        BNM_LOG_ERR("Field::GetFieldPointer dead instance: %s", str().c_str());
        return nullptr;
    } else if (_isStatic && !Utils::IsAllocated(_data->parent)) {
        BNM_LOG_ERR("Field::GetFieldPointer dead parent: %s", str().c_str());
        return nullptr;
    } else if (_isThreadStatic) {
        BNM_LOG_ERR("Field::GetFieldPointer thread static unsupported: %s", str().c_str());
        return nullptr;
    } else if (_isConst) {
        BNM_LOG_ERR("Field::GetFieldPointer const impossible: %s", str().c_str());
        return nullptr;
    }
    auto offset = (BNM_PTR) api.il2cpp_field_get_offset(_data);
    if (_isStatic) {
        auto staticData = api.il2cpp_class_get_static_field_data(_data->parent);
        return staticData ? (void *) ((BNM_PTR) staticData + offset) : nullptr;
    }
    return (void *) ((BNM_PTR) _instance + offset - (_isInStruct ? sizeof(IL2CPP::Il2CppObject) : 0));
}

BNM::Class FieldBase::GetType() const {
    if (!_data) return {};
    return Internal::api.il2cpp_field_get_type(_data);
}

BNM::Class FieldBase::GetParentClass() const {
    if (!_data) return {};
    return Internal::api.il2cpp_field_get_parent(_data);
}

PropertyBase::PropertyBase(const IL2CPP::PropertyInfo *info) {
    if (!info) return;

    _data = (IL2CPP::PropertyInfo *) info;

    auto get = Internal::api.il2cpp_property_get_get_method((IL2CPP::PropertyInfo *) info);
    auto set = Internal::api.il2cpp_property_get_set_method((IL2CPP::PropertyInfo *) info);

    if (get && get->methodPointer) {
        _hasGetter = true;
        _getter = get;
    }

    if (set && set->methodPointer) {
        _hasSetter = true;
        _setter = set;
    }
}

PropertyBase &PropertyBase::SetInstance(IL2CPP::Il2CppObject *val) {
    if (_hasGetter) _getter.SetInstance(val);
    if (_hasSetter) _setter.SetInstance(val);
    return *this;
}
