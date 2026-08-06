#pragma once

#include "Debug.hpp"
#include "Il2CppStructures.hpp"
#include "Internals.hpp"
#include "MonoStructures.hpp"
namespace BNM {
Structures::Mono::String *CreateMonoString(const std::string_view &str);
void *GetExternMethod(const std::string_view &str);
bool IsLoaded();
void *GetIl2CppLibraryHandle();
bool AttachIl2Cpp();
IL2CPP::Il2CppThread *CurrentIl2CppThread();
void DetachIl2Cpp();
void *Allocate(size_t size);
void Free(void *ptr);

template<typename T>
inline T UnmarshalUnityObject(BNM_INT_PTR ptr) { return (T) ptr; }

struct Class {
    IL2CPP::Il2CppClass *_data{};

    inline constexpr Class() = default;
    inline Class(const Class &) = default;
    inline Class(const IL2CPP::Il2CppClass *_class) : _data((IL2CPP::Il2CppClass *) _class) {}
    inline Class(const CompileTimeClass &compileTimeClass);
    Class(const IL2CPP::Il2CppObject *object);
    Class(const IL2CPP::Il2CppType *type);
    Class(const MonoType *type);
    Class(const std::string_view &_namespace, const std::string_view &name);
    Class(const std::string_view &_namespace, const std::string_view &name, const Image &image);

    std::vector<Class> GetInnerClasses(bool includeParent = false) const;
    std::vector<FieldBase> GetFields(bool includeParent = false) const;
    std::vector<MethodBase> GetMethods(bool includeParent = false) const;
    std::vector<PropertyBase> GetProperties(bool includeParent = false) const;
    std::vector<EventBase> GetEvents(bool includeParent = false) const;

    MethodBase GetMethod(const std::string_view &name, int parameters = -1) const;
    MethodBase GetMethod(const std::string_view &name, const std::initializer_list<std::string_view> &parameterNames) const;
    MethodBase GetMethod(const std::string_view &name, const std::initializer_list<CompileTimeClass> &parameterTypes) const;

    PropertyBase GetProperty(const std::string_view &name) const;
    PropertyBase GetProperty(const std::string_view &name, const CompileTimeClass &type) const;
    Class GetInnerClass(const std::string_view &name) const;
    FieldBase GetField(const std::string_view &name) const;
    EventBase GetEvent(const std::string_view &name) const;

    Class GetParent() const;
    Class GetArray() const;
    Class GetPointer() const;
    Class GetReference() const;
    Class GetGeneric(const std::initializer_list<CompileTimeClass> &templateTypes) const;

    IL2CPP::Il2CppType *GetIl2CppType() const;
    MonoType *GetMonoType() const;
    CompileTimeClass GetCompileTimeClass() const;
    Image GetImage() const;

    template<typename T>
    inline T *GetData() const { return (T *) _data; }
    template<typename T>
    inline Structures::Mono::Array<T> *NewArray(IL2CPP::il2cpp_array_size_t size) const {
        return (Structures::Mono::Array<T> *) Internal::api.il2cpp_array_new(_data, size);
    }
    inline IL2CPP::Il2CppClass *GetClass() const { return _data; }
    inline bool IsValid() const { return _data != nullptr; }
    inline operator bool() const { return IsValid(); }
    inline operator IL2CPP::Il2CppClass *() const { return GetClass(); }
    inline operator IL2CPP::Il2CppType *() const { return GetIl2CppType(); }
    inline operator MonoType *() const { return GetMonoType(); }
    inline operator CompileTimeClass() const;

    IL2CPP::Il2CppObject *CreateNewInstance() const;
    template<typename T>
    Structures::Mono::List<T> *NewListBNM() const {
        if (!_data) return nullptr;
        TryInit();
        auto *lst = (Structures::Mono::List<T> *) NewListInstance();
        if (!lst) return nullptr;
        lst->_items = NewArray<T>(1);
        lst->_size = 0;
        Structures::Mono::PRIVATE_MonoListData::InitMonoListVTable(lst);
        return lst;
    }
    template<typename T>
    Structures::Mono::List<T> *NewList() const { return NewListBNM<T>(); }
    template<typename ...Args>
    inline IL2CPP::Il2CppObject *CreateNewObjectParameters(Args &&...args) const;

    inline void TryInit() const { if (_data && Internal::classInitFunc) Internal::classInitFunc(_data); }

    static IL2CPP::Il2CppObject *BoxObject(IL2CPP::Il2CppClass *_data, void *data);
    static IL2CPP::Il2CppArray *ArrayNew(IL2CPP::Il2CppClass *cls, IL2CPP::il2cpp_array_size_t length);
    static void *NewListInstance();
    static Class GetListClass();

    inline std::string str() const {
        if (!_data) return "nullptr";
        std::string result;
        if (_data->namespaze && strlen(_data->namespaze)) {
            result += _data->namespaze;
            result += ".";
        }
        result += _data->name ? _data->name : "nullptr";
        return result;
    }
};

template<typename T, typename = std::enable_if<std::is_pointer_v<T>>>
bool IsA(T object, IL2CPP::Il2CppClass *_class) { return IsA<BNM::IL2CPP::Il2CppObject *>((IL2CPP::Il2CppObject *) object, _class); }

template<>
bool IsA<IL2CPP::Il2CppObject *>(IL2CPP::Il2CppObject *object, IL2CPP::Il2CppClass *_class);

template<typename T, typename = std::enable_if<std::is_pointer_v<T>>>
bool IsA(T object, Class _class) { return IsA(object, _class.GetClass()); }

template<typename T, typename = std::enable_if<std::is_pointer_v<T>>>
bool IsA(T object, IL2CPP::Il2CppObject *_object) { if (!_object) return false; return IsA(object, _object->klass); }

template<typename T, typename = std::enable_if<std::is_pointer_v<T>>>
bool IsA(T object, MonoType *_type) { return IsA(object, Class(_type)); }
}
