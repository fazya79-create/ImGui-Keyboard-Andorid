#include "BNM.hpp"

using namespace BNM;

namespace BNM::UnityEngine {
    BNM::Class UnityEventBase::GetArgumentType(PersistentCall *call) {
        auto type = Internal::vmData.UnityEngine$$Object;
        auto typeName = call->m_Arguments->m_ObjectArgumentAssemblyTypeName;
        if (!typeName->IsNullOrEmpty()) if (auto t = Internal::vmData.Type$$GetType.cast<Method<MonoType *>>()(typeName); t != nullptr) type = t;
        return type;
    }
    BNM::Class UnityEventBase::GetTargetType(PersistentCall *call) {
        BNM::Class targetType;
        if (call->m_Target) targetType = call->m_Target;
        else targetType = Internal::vmData.Type$$GetType.cast<Method<MonoType *>>()(call->m_TargetAssemblyTypeName);
        return targetType;
    }
}

MethodBase DelegateBase::GetMethod() const {
    auto method = MethodBase(this->method);
    auto instance = GetInstance();
    if (instance) method.SetInstance(instance);
    return method;
}

DelegateBase *DelegateBase::Create(BNM::MethodBase method) {
    return (DelegateBase *) BNM::Class(object.klass).CreateNewObjectParameters(method._instance, method._data);
}

std::vector<BNM::MethodBase> MulticastDelegateBase::GetMethods() const {
    auto delegates = (Structures::Mono::Array<DelegateBase *> *) this->delegates;
    if (!delegates) return {((DelegateBase *) this)->GetMethod()};

    std::vector<MethodBase> ret{};
    ret.reserve(delegates->max_length);
    for (IL2CPP::il2cpp_array_size_t i = 0; i < delegates->max_length; ++i) ret.push_back((*delegates)[i]->GetMethod());
    return ret;
}

void MulticastDelegateBase::Add(DelegateBase *delegate) {
    auto delegates = (Structures::Mono::Array<DelegateBase *> *) this->delegates;
    auto arr = BNM::Class(delegates->obj.klass->element_class).NewArray<DelegateBase *>(this->delegates->max_length + 1);
    arr->CopyFrom(delegates->GetItems(), delegates->max_length);
    arr->GetItems()[delegates->max_length] = delegate;
    this->delegates = (decltype(this->delegates)) arr;
}

void MulticastDelegateBase::Remove(DelegateBase *delegate) {
    auto delegates = (Structures::Mono::Array<DelegateBase *> *) this->delegates;
    IL2CPP::il2cpp_array_size_t index = 0;
    bool found = false;
    for (IL2CPP::il2cpp_array_size_t i = 0; i < delegates->max_length; ++i) {
        if ((*delegates)[i] != delegate) continue;
        found = true;
        index = i;
        break;
    }
    if (!found) return;
    auto arr = BNM::Class(delegates->obj.klass->element_class).NewArray<DelegateBase *>(this->delegates->max_length - 1);
    auto src = delegates->GetItems();
    auto dst = arr->GetItems();
    memmove(dst + index, src + index + 1, (delegates->max_length - index - 1) * sizeof(void *));
    if (index > 0) memcpy(dst, src, index * sizeof(void *));
    this->delegates = (decltype(this->delegates)) arr;
}

DelegateBase *MulticastDelegateBase::Add(BNM::MethodBase method) {
    auto delegate = ((DelegateBase *) this)->Create(method);
    Add(delegate);
    return delegate;
}
