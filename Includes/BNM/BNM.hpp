#pragma once
#if __cplusplus < 202002L
#error "BNM-Free requires C++20 and above!"
#endif

#include "Debug.hpp"
#include "Il2CppStructures.hpp"
#include "Internals.hpp"
#include "MonoStructures.hpp"
#include "UnityStructures.hpp"
#include "Class.hpp"
#include "MethodBase.hpp"
#include "FieldBase.hpp"
#include "PropertyBase.hpp"
#include "Image.hpp"
#include "Defaults.hpp"
#include "UnityEvent.hpp"
#include "CompileTimeClass.hpp"
#include "Management.hpp"
#include "Coroutine.hpp"

#ifdef BNM_USE_DOBBY
#include <dobby.h>
template<typename PTR_T, typename NEW_T, typename T_OLD>
inline void *BasicHook(PTR_T ptr, NEW_T newMethod, T_OLD &oldBytes) {
    if ((void *) ptr != nullptr) DobbyHook((void *) ptr, (dobby_dummy_func_t) newMethod, (dobby_dummy_func_t *) &oldBytes);
    return (void *) ptr;
}
template<typename PTR_T, typename NEW_T, typename T_OLD>
inline void *BasicHook(PTR_T ptr, NEW_T newMethod, T_OLD &&oldBytes) {
    if ((void *) ptr != nullptr) DobbyHook((void *) ptr, (dobby_dummy_func_t) newMethod, (dobby_dummy_func_t *) &oldBytes);
    return (void *) ptr;
}
template<typename PTR_T>
inline void Unhook(PTR_T ptr) {
    if ((void *) ptr != nullptr) DobbyDestroy((void *) ptr);
}
#elif defined(BNM_USE_SHADOWHOOK)
#include <shadowhook.h>
template<typename PTR_T, typename NEW_T, typename T_OLD>
inline void *BasicHook(PTR_T ptr, NEW_T newMethod, T_OLD &oldBytes) {
    if ((void *) ptr != nullptr) return shadowhook_hook_func_addr((void *) ptr, (void *) newMethod, (void **) &oldBytes);
    return nullptr;
}
template<typename PTR_T, typename NEW_T, typename T_OLD>
inline void *BasicHook(PTR_T ptr, NEW_T newMethod, T_OLD &&oldBytes) {
    if ((void *) ptr != nullptr) return shadowhook_hook_func_addr((void *) ptr, (void *) newMethod, (void **) &oldBytes);
    return nullptr;
}
template<typename PTR_T>
inline void Unhook(PTR_T ptr) {
    if ((void *) ptr != nullptr) shadowhook_unhook((void *) ptr);
}
#else
#error "No hooking software! Define BNM_USE_DOBBY or BNM_USE_SHADOWHOOK"
#endif

#define BNM_dlopen dlopen
#define BNM_dlsym dlsym
#define BNM_dlclose dlclose
#define BNM_dladdr dladdr

#define BNM_malloc malloc
#define BNM_free free

namespace BNM {

template<typename T_NEW, typename T_OLD>
inline bool VirtualHook(BNM::Class targetClass, const BNM::MethodBase &targetMethod, T_NEW newMet, T_OLD &oldMet) {
    return VirtualHookImpl(targetClass, targetMethod.GetInfo(), (void *) newMet, (void **) &oldMet);
}
template<typename T_NEW, typename T_OLD>
inline bool VirtualHook(BNM::Class targetClass, const BNM::MethodBase &targetMethod, T_NEW newMet, T_OLD &&oldMet) {
    return VirtualHookImpl(targetClass, targetMethod.GetInfo(), (void *) newMet, (void **) &oldMet);
}

namespace Internal {

struct VMData {
    BNM::Class Object{};
    BNM::Class UnityEngine$$Object{};
    BNM::Class System$$List{};
    BNM::MethodBase Type$$GetType{};
    BNM::MethodBase Interlocked$$CompareExchange{};
    BNM::MethodBase RuntimeType$$MakeGenericType{};
    BNM::MethodBase RuntimeType$$MakePointerType{};
    BNM::MethodBase RuntimeType$$make_byref_type{};
    BNM::MethodBase RuntimeMethodInfo$$MakeGenericMethod_impl{};
    IL2CPP::Il2CppString **String$$Empty{};
};

extern VMData vmData;

template<class CompareMethod>
IL2CPP::MethodInfo *IterateMethods(Class target, CompareMethod compare) {
    if (!target) return nullptr;
    void *iter = nullptr;
    while (auto method = api.il2cpp_class_get_methods(target._data, &iter)) {
        if (compare((IL2CPP::MethodInfo *) method)) return (IL2CPP::MethodInfo *) method;
    }
    auto parent = api.il2cpp_class_get_parent(target._data);
    if (parent) return IterateMethods(Class(parent), compare);
    return nullptr;
}

inline bool CompareImageName(IL2CPP::Il2CppImage *image, const std::string_view &name) {
    if (!image) return false;
    if (image->name && name == image->name) return true;
    if (image->nameNoExt && name == image->nameNoExt) return true;
    if (image->name) {
        std::string_view full(image->name);
        if (full.size() > 4 && full.substr(full.size() - 4) == ".dll" && full.substr(0, full.size() - 4) == name) return true;
    }
    return false;
}

inline std::vector<IL2CPP::Il2CppAssembly *> &GetAllAssemblies() {
    if (!assembliesCache.empty()) return assembliesCache;
    if (!api.il2cpp_domain_get) return assembliesCache;
    auto domain = api.il2cpp_domain_get();
    if (!domain) return assembliesCache;
    size_t size = 0;
    auto assemblies = api.il2cpp_domain_get_assemblies(domain, &size);
    for (size_t i = 0; i < size; ++i) if (assemblies[i]) assembliesCache.push_back(assemblies[i]);
    return assembliesCache;
}

}

inline Class::Class(const CompileTimeClass &compileTimeClass) { _data = compileTimeClass.ToClass()._data; }
inline Class::operator CompileTimeClass() const { return GetCompileTimeClass(); }
inline Defaults::DefaultTypeRef::operator BNM::CompileTimeClass() const {
    CompileTimeClass result;
    result._loadedClass = _reference ? *_reference : Class{};
    return result;
}
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-bool-conversion"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnonnull-compare"
#endif
inline std::string Structures::Mono::String::str() const {
    if (!this || !length) return {};
    std::string utf8;
    utf8.reserve(length);
    for (int32_t i = 0; i < length; ++i) {
        auto c = chars[i];
        if (c < 0x80) utf8 += (char) c;
        else if (c < 0x800) {
            utf8 += (char) (0xC0 | (c >> 6));
            utf8 += (char) (0x80 | (c & 0x3F));
        } else {
            utf8 += (char) (0xE0 | (c >> 12));
            utf8 += (char) (0x80 | ((c >> 6) & 0x3F));
            utf8 += (char) (0x80 | (c & 0x3F));
        }
    }
    return utf8;
}
#if defined(__clang__)
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
inline unsigned int Structures::Mono::String::GetHash() const {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-bool-conversion"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnonnull-compare"
#endif
    if (!this) return 0;
#if defined(__clang__)
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
    const IL2CPP::Il2CppChar *p = chars;
    unsigned int h = 0;
    for (int32_t i = 0; i < length; ++i) {
        h = (h << 5) - h + *p;
        ++p;
    }
    return h;
}
inline Structures::Unity::Vector2::operator Vector3() const { return {x, y, 0}; }
inline Structures::Unity::Vector3::operator Vector4() const { return {x, y, z, 0}; }
inline Structures::Unity::Vector4::Vector4(Color c) : x(c.r), y(c.g), z(c.b), w(c.a) {}

template<typename ...Args>
inline IL2CPP::Il2CppObject *Class::CreateNewObjectParameters(Args &&...args) const {
    auto obj = CreateNewInstance();
    if (!obj) return nullptr;
    auto ctor = GetMethod(Internal::constructorName, (int) sizeof...(Args));
    if (!ctor) return obj;
    auto method = ctor.GetInfo();
    ((void (*)(IL2CPP::Il2CppObject *, Args...)) method->methodPointer)(obj, std::forward<Args>(args)...);
    return obj;
}

}
