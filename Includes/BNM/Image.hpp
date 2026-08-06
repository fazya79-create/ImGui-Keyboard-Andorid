#pragma once

#include "Class.hpp"
namespace BNM {
struct Image {
    IL2CPP::Il2CppImage *_data{};
    inline size_t GetClassesCount() const { return _data ? _data->typeCount : -1; }

    inline constexpr Image() = default;
    inline Image(const Image &) = default;
    inline Image(const IL2CPP::Il2CppImage *image) : _data((IL2CPP::Il2CppImage *) image) {}
    Image(const std::string_view &name);
    Image(const IL2CPP::Il2CppAssembly *assembly);

    inline IL2CPP::Il2CppImage *GetInfo() const { return _data; }
    inline bool IsValid() const { return _data != nullptr; }
    inline operator IL2CPP::Il2CppImage *() const { return GetInfo(); }
    inline operator bool() const { return IsValid(); }
    inline std::string str() const { return _data ? (_data->name ? _data->name : "nullptr") : "nullptr"; }
    inline Class GetClass(const std::string_view &_namespace, const std::string_view &name) const { return Class(_namespace, name, *this); }
    std::vector<Class> GetClasses(bool includeInner = false) const;
    static std::vector<Image> GetImages();
};

template<typename Ret = void, typename ...Parameters>
struct Delegate;

struct DelegateBase {
    IL2CPP::Il2CppObject object;
    IL2CPP::MethodInfo *method;
    IL2CPP::Il2CppObject *target;

    MethodBase GetMethod() const;
    DelegateBase *Create(BNM::MethodBase method);
    inline IL2CPP::Il2CppObject *GetInstance() const { return target; }
};

struct MulticastDelegateBase {
    IL2CPP::Il2CppObject object;
    IL2CPP::Il2CppMethodPointer method_ptr;
    IL2CPP::Il2CppObject *target;
    IL2CPP::Il2CppArray *delegates;

    std::vector<MethodBase> GetMethods() const;
    void Add(DelegateBase *delegate);
    void Remove(DelegateBase *delegate);
    DelegateBase *Add(BNM::MethodBase method);
};

template<typename Ret, typename ...Parameters>
struct Delegate : public MulticastDelegateBase {
    inline Delegate() = default;
    inline Delegate(const Delegate &) = default;
    inline Ret Invoke(Parameters ...parameters) const {
        if (!method_ptr) {
            BNM_LOG_ERR("Delegate::Invoke dead delegate");
            return PRIVATE_INTERNAL::ReturnEmpty<Ret>();
        }
        return ((Ret(*)(Parameters...)) method_ptr)(parameters...);
    }
    inline Ret operator()(Parameters ...parameters) const { return Invoke(parameters...); }
};
}
