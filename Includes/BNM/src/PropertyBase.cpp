#include "BNM.hpp"

using namespace BNM;

BNM::Class PropertyBase::GetType() const {
    if (!_data) return {};
    auto get = Internal::api.il2cpp_property_get_get_method(_data);
    if (get) return Internal::api.il2cpp_method_get_return_type(get);
    auto set = Internal::api.il2cpp_property_get_set_method(_data);
    if (set) return Internal::api.il2cpp_method_get_param(set, 0);
    return {};
}

BNM::Class PropertyBase::GetParentClass() const {
    if (!_data) return {};
    return Internal::api.il2cpp_property_get_parent(_data);
}

EventBase::EventBase(const IL2CPP::EventInfo *info) {
    if (!info) return;
    _hasAdd = _hasRemove = _hasRaise = false;
    if (info->add && info->add->methodPointer) {
        _hasAdd = true;
        _add = info->add;
    }
    if (info->remove && info->remove->methodPointer) {
        _hasRemove = true;
        _remove = info->remove;
    }
    if (info->raise && info->raise->methodPointer) {
        _hasRaise = true;
        _raise = info->raise;
    }
    _data = (IL2CPP::EventInfo *) info;
}

EventBase &EventBase::SetInstance(IL2CPP::Il2CppObject *instance) {
    if (_hasAdd) _add.SetInstance(instance);
    if (_hasRemove) _remove.SetInstance(instance);
    if (_hasRaise) _raise.SetInstance(instance);
    return *this;
}

BNM::Class EventBase::GetParentClass() const {
    if (!_data) return {};
    return _data->parent;
}
