#pragma once

#include "Class.hpp"
#include "Internals.hpp"
namespace BNM {
namespace MANAGEMENT_STRUCTURES {

struct CustomMethod {
    std::string _name{};
    std::string _copyTarget{};
    std::vector<CompileTimeClass> _parameterTypes{};
    CompileTimeClass _returnType{};
    void *_address{};
    void *_invoker{};
    uint8_t _isStatic : 1 = false;
    uint8_t _isInvokeHook : 1 = false;
    uint8_t _isBasicHook : 1 = false;
    uint8_t _skipTypeMatch : 1 = false;
    IL2CPP::MethodInfo *myInfo{};
    IL2CPP::MethodInfo *_origin{};
    void *_originalAddress{};
    uint16_t _slot{};
};

struct CustomField {
    std::string _name{};
    CompileTimeClass _type{};
    int32_t offset{};
    size_t _size{};
    IL2CPP::FieldInfo *myInfo{};
};

struct CustomClass {
    CompileTimeClass _targetType{};
    CompileTimeClass _baseType{};
    IL2CPP::Il2CppClass *_owner{};
    std::vector<CompileTimeClass> _interfaces{};
    std::vector<CustomMethod *> _methods{};
    std::vector<CustomField *> _fields{};
    size_t _size{};
    IL2CPP::Il2CppClass *myClass{};
    Class type{};
    void *userData{};
};

template<bool IsStatic, typename MET>
struct GetMethodInvoker;

template<typename Ret, typename CLS, typename ...Args>
struct GetMethodInvoker<false, Ret (CLS::*)(Args...)> {
    static void Invoke(IL2CPP::Il2CppMethodPointer ptr, const IL2CPP::MethodInfo *, void *obj, void **params, void *) {
        Ret (CLS::*fn)(Args...) = nullptr;
        memcpy(&fn, &ptr, sizeof(fn));
        if constexpr (std::is_void_v<Ret>) ((CLS *) obj->*fn)((Args) *params...);
        else ((CLS *) obj->*fn)((Args) *params...);
    }
};

template<typename Ret, typename CLS, typename ...Args>
struct GetMethodInvoker<false, Ret (CLS::*)(Args...) const> {
    static void Invoke(IL2CPP::Il2CppMethodPointer ptr, const IL2CPP::MethodInfo *, void *obj, void **params, void *) {
        Ret (CLS::*fn)(Args...) const = nullptr;
        memcpy(&fn, &ptr, sizeof(fn));
        if constexpr (std::is_void_v<Ret>) ((CLS *) obj->*fn)((Args) *params...);
        else ((CLS *) obj->*fn)((Args) *params...);
    }
};

template<typename Ret, typename CLS, typename ...Args>
struct GetMethodInvoker<true, Ret (CLS::*)(Args...)> {
    static void Invoke(IL2CPP::Il2CppMethodPointer ptr, const IL2CPP::MethodInfo *, void *, void **params, void *) {
        if constexpr (std::is_void_v<Ret>) ((Ret (*)(Args...)) ptr)((Args) *params...);
        else ((Ret (*)(Args...)) ptr)((Args) *params...);
    }
};

template<typename Ret, typename ...Args>
struct GetMethodInvoker<false, Ret (*)(Args...)> {
    static void Invoke(IL2CPP::Il2CppMethodPointer ptr, const IL2CPP::MethodInfo *, void *, void **params, void *) {
        if constexpr (std::is_void_v<Ret>) ((Ret (*)(Args...)) ptr)((Args) *params...);
        else ((Ret (*)(Args...)) ptr)((Args) *params...);
    }
};

template<typename Ret, typename ...Args>
struct GetMethodInvoker<true, Ret (*)(Args...)> {
    static void Invoke(IL2CPP::Il2CppMethodPointer ptr, const IL2CPP::MethodInfo *, void *, void **params, void *) {
        if constexpr (std::is_void_v<Ret>) ((Ret (*)(Args...)) ptr)((Args) *params...);
        else ((Ret (*)(Args...)) ptr)((Args) *params...);
    }
};

template<typename Ret, typename ...Args>
struct GetMethodInvoker<false, Ret(Args...)> {
    static void Invoke(IL2CPP::Il2CppMethodPointer ptr, const IL2CPP::MethodInfo *, void *obj, void **params, void *) {
        if constexpr (std::is_void_v<Ret>) ((Ret (*)(Args...)) ptr)((Args) *params...);
        else ((Ret (*)(Args...)) ptr)((Args) *params...);
    }
};

template<typename Ret, typename ...Args>
struct GetMethodInvoker<true, Ret(Args...)> {
    static void Invoke(IL2CPP::Il2CppMethodPointer ptr, const IL2CPP::MethodInfo *, void *, void **params, void *) {
        if constexpr (std::is_void_v<Ret>) ((Ret (*)(Args...)) ptr)((Args) *params...);
        else ((Ret (*)(Args...)) ptr)((Args) *params...);
    }
};

void AddClass(CustomClass *_class);

}

namespace ClassesManagement {

extern std::vector<MANAGEMENT_STRUCTURES::CustomClass *> *classesManagementVector;

void ProcessCustomClasses();
void ProcessClassRuntime(MANAGEMENT_STRUCTURES::CustomClass *customClass);
void ForEachClassInImage(IL2CPP::Il2CppImage *image, const std::function<bool(IL2CPP::Il2CppClass *)> &func);

IL2CPP::Il2CppClass *Class$$FromIl2CppType(IL2CPP::Il2CppType *type);
IL2CPP::Il2CppClass *Type$$GetClassOrElementClass(IL2CPP::Il2CppType *type);
IL2CPP::Il2CppClass *Class$$FromName(IL2CPP::Il2CppImage *image, const char *namespace_, const char *name);

extern IL2CPP::Il2CppClass *(*old_Class$$FromIl2CppType)(IL2CPP::Il2CppType *type);
extern IL2CPP::Il2CppClass *(*old_Type$$GetClassOrElementClass)(IL2CPP::Il2CppType *type);
extern IL2CPP::Il2CppClass *(*old_Class$$FromName)(IL2CPP::Il2CppImage *image, const char *namespace_, const char *name);

}
}
