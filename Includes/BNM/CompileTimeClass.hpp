#pragma once

#include "Class.hpp"
namespace BNM {
struct CompileTimeClass {
    enum class ModifierType : uint8_t { None = 0, Array = 1, Pointer = 2, Reference = 3 };
    enum class _BaseType : uint8_t { None = 0, Class = 1, Inner = 2, Modifier = 3, Generic = 4, MaxCount = 5 };

    struct _BaseInfo {
        _BaseType _baseType{};
        inline constexpr _BaseInfo(_BaseType type) : _baseType(type) {}
    };

    struct _ClassInfo : _BaseInfo {
        const char *_namespace{};
        const char *_name{};
        const char *_imageName{};
        inline constexpr _ClassInfo() : _BaseInfo(_BaseType::Class) {}
    };

    struct _InnerInfo : _BaseInfo {
        const char *_name{};
        inline constexpr _InnerInfo() : _BaseInfo(_BaseType::Inner) {}
    };

    struct _ModifierInfo : _BaseInfo {
        ModifierType _modifierType{};
        inline constexpr _ModifierInfo() : _BaseInfo(_BaseType::Modifier) {}
    };

    struct _GenericInfo : _BaseInfo {
        std::vector<CompileTimeClass> _types{};
        inline _GenericInfo() : _BaseInfo(_BaseType::Generic) {}
    };

    Class _loadedClass{};
    std::vector<_BaseInfo *> _stack{};
    Class *_reference{};
    uint8_t _autoFree : 1 = false;

    inline CompileTimeClass() = default;
    inline CompileTimeClass(Class *_ref) : _reference(_ref) { _autoFree = true; }
    inline CompileTimeClass(const CompileTimeClass &other) = default;
    inline ~CompileTimeClass() { if (_autoFree) Free(); }

    inline void AddClass(const char *_namespace, const char *name, const char *imageName = nullptr) {
        auto info = new _ClassInfo();
        info->_namespace = _namespace;
        info->_name = name;
        info->_imageName = imageName;
        _stack.push_back(info);
    }
    inline void AddInnerClass(const char *name) {
        auto info = new _InnerInfo();
        info->_name = name;
        _stack.push_back(info);
    }
    inline void AddModifier(ModifierType type) {
        auto info = new _ModifierInfo();
        info->_modifierType = type;
        _stack.push_back(info);
    }
    inline void AddGeneric(const std::vector<CompileTimeClass> &types) {
        auto info = new _GenericInfo();
        info->_types = types;
        _stack.push_back(info);
    }

    Class ToClass();
    inline Class ToClass() const { return ((CompileTimeClass *) this)->ToClass(); }
    inline operator Class() const { return ToClass(); }
    inline IL2CPP::Il2CppType *ToIl2CppType() const { return ToClass().GetIl2CppType(); }
    inline operator IL2CPP::Il2CppType *() const { return ToIl2CppType(); }
    inline IL2CPP::Il2CppClass *ToIl2CppClass() const { return ToClass().GetClass(); }
    inline operator IL2CPP::Il2CppClass *() const { return ToIl2CppClass(); }
    inline BNM::MethodBase operator[](const std::string_view &name) const { return ToClass().GetMethod(name); }
    inline BNM::FieldBase operator[](const char *name) const { return ToClass().GetField(name); }
    inline void Free() {
        for (auto info : _stack) delete info;
        _stack.clear();
        _autoFree = false;
    }
};

struct CompileTimeClassBuilder {
    CompileTimeClass _class{};
    inline CompileTimeClassBuilder(const char *_namespace, const char *name) { _class.AddClass(_namespace, name); }
    inline CompileTimeClassBuilder(const char *_namespace, const char *name, const char *imageName) { _class.AddClass(_namespace, name, imageName); }
    inline CompileTimeClassBuilder &Inner(const char *name) { _class.AddInnerClass(name); return *this; }
    inline CompileTimeClassBuilder &Pointer() { _class.AddModifier(CompileTimeClass::ModifierType::Pointer); return *this; }
    inline CompileTimeClassBuilder &Reference() { _class.AddModifier(CompileTimeClass::ModifierType::Reference); return *this; }
    inline CompileTimeClassBuilder &Array() { _class.AddModifier(CompileTimeClass::ModifierType::Array); return *this; }
    inline CompileTimeClassBuilder &Generic(const std::vector<CompileTimeClass> &types) { _class.AddGeneric(types); return *this; }
    inline CompileTimeClass Build() { return _class; }
};
}
