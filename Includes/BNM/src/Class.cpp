#include "BNM.hpp"

using namespace BNM;

Class::Class(const IL2CPP::Il2CppObject *object) {
    if (!object) return;
    _data = object->klass;
}

Class::Class(const IL2CPP::Il2CppType *type) {
    if (!type) return;
    _data = Internal::api.il2cpp_class_from_il2cpp_type((IL2CPP::Il2CppType *) type);
}

Class::Class(const MonoType *type) {
    if (!type) return;
    _data = Internal::api.il2cpp_class_from_il2cpp_type((IL2CPP::Il2CppType *) type->type);
}

IL2CPP::Il2CppClass *BNM::Internal::TryGetClassWithoutImage(const std::string_view &_namespace, const std::string_view &_name) {
    auto &assemblies = Internal::GetAllAssemblies();

    for (auto assembly : assemblies) {
        if (!assembly) continue;
        auto image = Internal::api.il2cpp_assembly_get_image(assembly);
        if (auto _data = Internal::TryGetClassInImage(image, _namespace, _name); _data) return _data;
    }

    return nullptr;
}

Class::Class(const std::string_view &_namespace, const std::string_view &_name) {
    if (_data = Internal::TryGetClassWithoutImage(_namespace, _name); _data) return;
    BNM_LOG_WARN("Class not found: %s.%s", _namespace.data(), _name.data());
}

Class::Class(const std::string_view &_namespace, const std::string_view &_name, const BNM::Image &image) {
    if (!image) {
        BNM_LOG_WARN("Class image invalid: %s for %s.%s", image.str().data(), _namespace.data(), _name.data());
        _data = nullptr;
        return;
    }

    if (_data = Internal::TryGetClassInImage(image, _namespace, _name); _data) return;

    BNM_LOG_WARN("Class not found in image: %s.%s in %s", _namespace.data(), _name.data(), image.str().data());
}

std::vector<Class> Class::GetInnerClasses(bool includeParent) const {
    if (!_data) return {};
    TryInit();
    std::vector<Class> ret{};
    auto curClass = _data;

    do {
        void *iter = nullptr;
        while (auto cls = Internal::api.il2cpp_class_get_nested_types(curClass, &iter)) ret.emplace_back(cls);
        if (includeParent) curClass = Internal::api.il2cpp_class_get_parent(curClass);
        else break;
    } while (curClass);

    return ret;
}

std::vector<FieldBase> Class::GetFields(bool includeParent) const {
    if (!_data) return {};
    TryInit();
    std::vector<FieldBase> ret{};
    auto curClass = _data;

    do {
        void *iter = nullptr;
        while (auto field = Internal::api.il2cpp_class_get_fields(curClass, &iter)) ret.emplace_back((IL2CPP::FieldInfo *) field);
        if (includeParent) curClass = Internal::api.il2cpp_class_get_parent(curClass);
        else break;
    } while (curClass);

    return ret;
}

std::vector<MethodBase> Class::GetMethods(bool includeParent) const {
    if (!_data) return {};
    TryInit();
    std::vector<MethodBase> ret{};
    auto curClass = _data;

    do {
        void *iter = nullptr;
        while (auto method = Internal::api.il2cpp_class_get_methods(curClass, &iter)) ret.emplace_back((IL2CPP::MethodInfo *) method);
        if (includeParent) curClass = Internal::api.il2cpp_class_get_parent(curClass);
        else break;
    } while (curClass);

    return ret;
}

std::vector<PropertyBase> Class::GetProperties(bool includeParent) const {
    if (!_data) return {};
    TryInit();
    std::vector<PropertyBase> ret{};
    auto curClass = _data;

    do {
        void *iter = nullptr;
        while (auto prop = Internal::api.il2cpp_class_get_properties(curClass, &iter)) ret.emplace_back((IL2CPP::PropertyInfo *) prop);
        if (includeParent) curClass = Internal::api.il2cpp_class_get_parent(curClass);
        else break;
    } while (curClass);

    return ret;
}

std::vector<EventBase> Class::GetEvents(bool includeParent) const {
    if (!_data) return {};
    TryInit();
    std::vector<EventBase> ret{};
    auto curClass = _data;

    do {
        void *iter = nullptr;
        while (auto eventInfo = Internal::api.il2cpp_class_get_events(curClass, &iter)) ret.emplace_back((IL2CPP::EventInfo *) eventInfo);
        if (includeParent) curClass = Internal::api.il2cpp_class_get_parent(curClass);
        else break;
    } while (curClass);

    return ret;
}

MethodBase Class::GetMethod(const std::string_view &name, int parameters) const {
    if (!_data) return {};
    TryInit();

    auto method = Internal::IterateMethods(*this, [&name, parameters](IL2CPP::MethodInfo *method) {
        return name == (method->name ? method->name : "") && (method->parameters_count == parameters || parameters == -1);
    });

    if (method != nullptr) return method;

    BNM_LOG_WARN("Method not found: %s.%s(%d)", _data->namespaze ? _data->namespaze : "", _data->name ? _data->name : "", parameters);
    return {};
}

MethodBase Class::GetMethod(const std::string_view &name, const std::initializer_list<std::string_view> &parameterNames) const {
    if (!_data) return {};
    TryInit();

    auto parameters = (uint8_t) parameterNames.size();

    auto method = Internal::IterateMethods(*this, [&name, &parameters, &parameterNames](IL2CPP::MethodInfo *method) {
        if (name != (method->name ? method->name : "") || method->parameters_count != parameters) return false;
        for (uint8_t i = 0; i < parameters; ++i) if (Internal::api.il2cpp_method_get_param_name(method, i) != parameterNames.begin()[i]) return false;
        return true;
    });

    if (method != nullptr) return method;

    BNM_LOG_WARN("Method not found by names: %s", name.data());
    return {};
}

MethodBase Class::GetMethod(const std::string_view &name, const std::initializer_list<CompileTimeClass> &parameterTypes) const {
    if (!_data) return {};
    TryInit();
    auto parameters = (uint8_t) parameterTypes.size();

    auto method = Internal::IterateMethods(*this, [&name, parameters, &parameterTypes](IL2CPP::MethodInfo *method) {
        if (name != (method->name ? method->name : "") || method->parameters_count != parameters) return false;
        for (uint8_t i = 0; i < parameters; ++i) {
            auto param = Internal::api.il2cpp_method_get_param(method, i);
            if (!param) return false;
            if (Class(param).GetClass() != parameterTypes.begin()[i].ToIl2CppClass()) return false;
        }
        return true;
    });

    if (method != nullptr) return method;

    BNM_LOG_WARN("Method not found by types: %s", name.data());
    return {};
}

PropertyBase Class::GetProperty(const std::string_view &name) const {
    if (!_data) return {};
    TryInit();
    auto curClass = _data;

    do {
        auto prop = Internal::api.il2cpp_class_get_property_from_name(curClass, name.data());
        if (prop) return prop;
        curClass = Internal::api.il2cpp_class_get_parent(curClass);
    } while (curClass);

    BNM_LOG_WARN("Property not found: %s", name.data());
    return {};
}

PropertyBase Class::GetProperty(const std::string_view &name, const CompileTimeClass &type) const {
    if (!_data) return {};
    TryInit();
    auto curClass = _data;

    auto typeClass = type.ToIl2CppClass();

    do {
        void *iter = nullptr;
        while (auto currentProperty = Internal::api.il2cpp_class_get_properties(curClass, &iter)) {
            auto propInfo = (IL2CPP::PropertyInfo *) currentProperty;
            if (name == (propInfo->name ? propInfo->name : "") && PropertyBase{propInfo}.GetType() == typeClass) return propInfo;
        }
        curClass = Internal::api.il2cpp_class_get_parent(curClass);
    } while (curClass);

    BNM_LOG_WARN("Property not found by type: %s", name.data());
    return {};
}

Class Class::GetInnerClass(const std::string_view &name) const {
    if (!_data) return {};
    TryInit();
    auto curClass = _data;

    do {
        void *iter = nullptr;
        while (auto cls = Internal::api.il2cpp_class_get_nested_types(curClass, &iter)) {
            auto clsName = Internal::api.il2cpp_class_get_name(cls);
            if (name == (clsName ? clsName : "")) return cls;
        }
        curClass = Internal::api.il2cpp_class_get_parent(curClass);
    } while (curClass);

    BNM_LOG_WARN("Inner class not found: %s", name.data());
    return {};
}

FieldBase Class::GetField(const std::string_view &name) const {
    if (!_data) return {};
    TryInit();
    auto curClass = _data;

    do {
        auto field = Internal::api.il2cpp_class_get_field_from_name(curClass, name.data());
        if (field) return field;
        curClass = Internal::api.il2cpp_class_get_parent(curClass);
    } while (curClass);

    BNM_LOG_WARN("Field not found: %s", name.data());
    return {};
}

EventBase Class::GetEvent(const std::string_view &name) const {
    if (!_data) return {};
    TryInit();
    auto curClass = _data;

    do {
        void *iter = nullptr;
        while (auto currentEvent = Internal::api.il2cpp_class_get_events(curClass, &iter)) {
            auto eventInfo = (IL2CPP::EventInfo *) currentEvent;
            if (name == (eventInfo->name ? eventInfo->name : "")) return eventInfo;
        }
        curClass = Internal::api.il2cpp_class_get_parent(curClass);
    } while (curClass);

    BNM_LOG_WARN("Event not found: %s", name.data());
    return {};
}

Class Class::GetParent() const {
    if (!_data) return {};
    TryInit();
    return Internal::api.il2cpp_class_get_parent(_data);
}

Class Class::GetArray() const {
    if (!_data) return {};
    TryInit();
    return Internal::api.il2cpp_array_class_get(_data, 1);
}

Class Class::GetPointer() const {
    if (!_data) return {};
    TryInit();
    return Internal::GetPointer(*this);
}

Class Class::GetReference() const {
    if (!_data) return {};
    TryInit();
    return Internal::GetReference(*this);
}

Class Class::GetGeneric(const std::initializer_list<CompileTimeClass> &templateTypes) const {
    if (!_data) return {};
    TryInit();
    return Internal::TryMakeGenericClass(*this, templateTypes);
}

IL2CPP::Il2CppType *Class::GetIl2CppType() const {
    if (!_data) return nullptr;
    TryInit();
    return Internal::api.il2cpp_class_get_type(_data);
}

MonoType *Class::GetMonoType() const {
    if (!_data) return nullptr;
    TryInit();
    return (MonoType *) Internal::api.il2cpp_type_get_object(GetIl2CppType());
}

CompileTimeClass Class::GetCompileTimeClass() const {
    TryInit();
    CompileTimeClass result{};
    result._loadedClass = *this;
    return result;
}

Image Class::GetImage() const {
    if (!_data) return {};
    TryInit();
    return {Internal::api.il2cpp_class_get_image(_data)};
}

IL2CPP::Il2CppObject *Class::CreateNewInstance() const {
    if (!_data) return {};
    TryInit();

    if ((Internal::api.il2cpp_class_get_flags(_data) & (0x00000080 | 0x00000020)))
        BNM_LOG_WARN("CreateNewInstance on abstract/interface: %s", str().c_str());

    auto obj = Internal::api.il2cpp_object_new(_data);
    if (obj) memset((char *) obj + sizeof(IL2CPP::Il2CppObject), 0, Internal::api.il2cpp_class_instance_size(_data) - sizeof(IL2CPP::Il2CppObject));
    return obj;
}

IL2CPP::Il2CppObject *Class::BoxObject(IL2CPP::Il2CppClass *_data, void *data) {
    return Internal::api.il2cpp_value_box(_data, data);
}

IL2CPP::Il2CppArray *Class::ArrayNew(IL2CPP::Il2CppClass *cls, IL2CPP::il2cpp_array_size_t length) {
    return Internal::api.il2cpp_array_new(cls, length);
}

void *Class::NewListInstance() {
    return Internal::customListTemplateClass ? Internal::api.il2cpp_object_new(Internal::customListTemplateClass) : nullptr;
}

Class Class::GetListClass() {
    return Internal::vmData.System$$List;
}

Class CompileTimeClass::ToClass() {
    if (!_autoFree) {
        if (_loadedClass) return _loadedClass;
        if (_stack.empty()) return {};
    }

    if (_stack.empty()) {
        if (_reference) {
            _loadedClass = *_reference;
            _autoFree = false;
            _loadedClass.TryInit();
            return _loadedClass;
        }
        return _loadedClass;
    }

    bool autoFree = _autoFree;
    _autoFree = false;

    for (auto info : _stack) {
        switch (info->_baseType) {
            case _BaseType::Class: {
                auto classInfo = (_ClassInfo *) info;
                auto _namespace = classInfo->_namespace ? classInfo->_namespace : "";
                if (!classInfo->_imageName || !strlen(classInfo->_imageName)) {
                    _loadedClass = Internal::TryGetClassWithoutImage(_namespace, classInfo->_name);
                    break;
                }
                BNM::Image image{};
                auto &assemblies = Internal::GetAllAssemblies();
                for (auto assembly : assemblies) {
                    if (!assembly) continue;
                    auto currentImage = Internal::api.il2cpp_assembly_get_image(assembly);
                    if (!Internal::CompareImageName(currentImage, classInfo->_imageName)) continue;
                    image = currentImage;
                    break;
                }
                _loadedClass = Internal::TryGetClassInImage(image, _namespace, classInfo->_name);
            } break;
            case _BaseType::Inner: {
                auto innerInfo = (_InnerInfo *) info;
                if (!_loadedClass) {
                    BNM_LOG_WARN("Inner class parent not resolved: %s", innerInfo->_name);
                    break;
                }
                _loadedClass = _loadedClass.GetInnerClass(innerInfo->_name);
            } break;
            case _BaseType::Modifier: {
                auto modifierInfo = (_ModifierInfo *) info;
                switch (modifierInfo->_modifierType) {
                    case ModifierType::Pointer: _loadedClass = _loadedClass.GetPointer(); break;
                    case ModifierType::Reference: _loadedClass = _loadedClass.GetReference(); break;
                    case ModifierType::Array: _loadedClass = _loadedClass.GetArray(); break;
                    case ModifierType::None: break;
                }
            } break;
            case _BaseType::Generic: {
                auto genericInfo = (_GenericInfo *) info;
                if (!_loadedClass) {
                    BNM_LOG_WARN("Generic parent not resolved");
                    break;
                }
                _loadedClass = Internal::TryMakeGenericClass(_loadedClass, genericInfo->_types);
            } break;
            case _BaseType::None:
            case _BaseType::MaxCount: break;
        }
    }

    if (autoFree) Free();

    _loadedClass.TryInit();
    return _loadedClass;
}
