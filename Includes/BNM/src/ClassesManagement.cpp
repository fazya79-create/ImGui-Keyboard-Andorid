#include "BNM.hpp"

using namespace BNM;

void MANAGEMENT_STRUCTURES::AddClass(CustomClass *_class) {
    if (!ClassesManagement::classesManagementVector)
        ClassesManagement::classesManagementVector = new (BNM_malloc(sizeof(std::vector<MANAGEMENT_STRUCTURES::CustomClass *>))) std::vector<MANAGEMENT_STRUCTURES::CustomClass *>();
    ClassesManagement::classesManagementVector->push_back(_class);
}

namespace BNM::ClassesManagement {

static constexpr BNM_PTR BNM_CLASS_ALLOCATED_METHODS_FLAG = 0x01000000;
static constexpr BNM_PTR BNM_CLASS_ALLOCATED_FIELDS_FLAG = 0x02000000;
static constexpr BNM_PTR BNM_CLASS_ALLOCATED_INNER_FLAG = 0x04000000;
static constexpr BNM_PTR BNM_CLASS_ALLOCATED_HIERARCHY_FLAG = 0x08000000;

std::vector<MANAGEMENT_STRUCTURES::CustomClass *> *classesManagementVector = nullptr;
static std::map<BNM_PTR, std::vector<IL2CPP::Il2CppClass *>> bnmClassesMap{};

static void ForEachClassInImageImpl(IL2CPP::Il2CppImage *image, const std::function<bool(IL2CPP::Il2CppClass *)> &func) {
    auto it = bnmClassesMap.find((BNM_PTR) image);
    if (it == bnmClassesMap.end()) return;
    for (auto cls : it->second) if (func(cls)) break;
}

void ForEachClassInImage(IL2CPP::Il2CppImage *image, const std::function<bool(IL2CPP::Il2CppClass *)> &func) {
    ForEachClassInImageImpl(image, func);
}

static void AddClassToMap(IL2CPP::Il2CppImage *image, IL2CPP::Il2CppClass *cls) {
    bnmClassesMap[(BNM_PTR) image].emplace_back(cls);
}

#define BNM_I2C_NEW(type) (IL2CPP::type *) BNM_malloc(sizeof(IL2CPP::type))

struct CustomClassInfo {
    const char *_namespace{};
    const char *_name{};
    const char *_imageName{};
    Class _class{};
};

static void ModifyClass(MANAGEMENT_STRUCTURES::CustomClass *customClass, Class target);
static void CreateClass(MANAGEMENT_STRUCTURES::CustomClass *customClass, const CustomClassInfo &classInfo);
static CustomClassInfo GetClassInfo(const CompileTimeClass &compileTimeClass);
static IL2CPP::Il2CppImage *MakeImage(std::string_view imageName);
static IL2CPP::MethodInfo *ProcessCustomMethod(MANAGEMENT_STRUCTURES::CustomMethod *method, Class target, bool *hooked = nullptr);
static IL2CPP::MethodInfo *CreateMethod(MANAGEMENT_STRUCTURES::CustomMethod *method);
static void SetupField(IL2CPP::FieldInfo *newField, MANAGEMENT_STRUCTURES::CustomField *field);
static void GetAllInterfaces(IL2CPP::Il2CppClass *parent, IL2CPP::Il2CppClass *interface, std::vector<IL2CPP::Il2CppClass *> &outInterfaces);
static void SetupClassOwner(IL2CPP::Il2CppClass *target, IL2CPP::Il2CppClass *owner);
static void SetupClassParent(IL2CPP::Il2CppClass *target, IL2CPP::Il2CppClass *parent);
static bool HasInterface(IL2CPP::Il2CppClass *parent, IL2CPP::Il2CppClass *interface);
static void SetupTypes(IL2CPP::Il2CppClass *target);

void ProcessCustomClasses() {
    if (classesManagementVector == nullptr) return;

    for (auto customClass : *classesManagementVector) ProcessClassRuntime(customClass);

    classesManagementVector->clear();
    classesManagementVector->shrink_to_fit();
    BNM_free((void *) classesManagementVector);
    classesManagementVector = nullptr;
}

void ProcessClassRuntime(MANAGEMENT_STRUCTURES::CustomClass *customClass) {
    auto &type = customClass->_targetType;
    auto info = GetClassInfo(type);
    if (info._class) ModifyClass(customClass, info._class);
    else if (info._name) CreateClass(customClass, info);
    else {
        BNM_LOG_ERR("ProcessClassRuntime failed to resolve target");
    }
    type.Free();
    customClass->_interfaces.clear();
    customClass->_interfaces.shrink_to_fit();
    customClass->_fields.clear();
    customClass->_fields.shrink_to_fit();
    customClass->_methods.clear();
    customClass->_methods.shrink_to_fit();
}

static void ModifyClass(MANAGEMENT_STRUCTURES::CustomClass *customClass, Class target) {
    BNM_LOG_DEBUG("ModifyClass target: %s", target.str().data());

    auto klass = target._data;

    auto baseType = customClass->_baseType.ToClass();

    IL2CPP::Il2CppClass *owner = customClass->_owner;

    SetupClassParent(klass, baseType._data);
    SetupClassOwner(klass, owner);

    auto newMethodsCount = customClass->_methods.size();
    auto newFieldsCount = customClass->_fields.size();

    if (newMethodsCount) {
        auto oldCount = klass->method_count;
        auto oldMethods = klass->methods;

        std::vector<IL2CPP::MethodInfo *> methodsToAdd{};

        for (size_t i = 0; i < newMethodsCount; ++i) {
            auto method = customClass->_methods[i];

            auto paramCount = method->_parameterTypes.size();
            (void) paramCount;

            bool isHooked = false;
            method->myInfo = ProcessCustomMethod(method, target, &isHooked);

            PRIVATE_INTERNAL::GetMethodClass(method->myInfo) = klass;

            if (!isHooked) methodsToAdd.push_back(method->myInfo);
            BNM_LOG_DEBUG_IF(isHooked, "ModifyClass hooked method: %s (%zu)", method->_name.data(), paramCount);
            BNM_LOG_DEBUG_IF(!isHooked, "ModifyClass added method: %s (%zu)", method->_name.data(), paramCount);
        }

        if (!methodsToAdd.empty()) {
            auto newMethods = (IL2CPP::MethodInfo **) BNM_malloc((oldCount + methodsToAdd.size()) * sizeof(IL2CPP::MethodInfo *));

            auto oldSize = oldCount * sizeof(IL2CPP::MethodInfo *);

            if (oldCount) memcpy(newMethods, oldMethods, oldSize);

            memcpy(newMethods + oldCount, methodsToAdd.data(), methodsToAdd.size() * sizeof(IL2CPP::MethodInfo *));

            if ((klass->flags & BNM_CLASS_ALLOCATED_METHODS_FLAG) == BNM_CLASS_ALLOCATED_METHODS_FLAG) BNM_free((void *) klass->methods);
            klass->flags |= (uint32_t) BNM_CLASS_ALLOCATED_METHODS_FLAG;

            klass->methods = (const IL2CPP::MethodInfo **) newMethods;
            klass->method_count += methodsToAdd.size();
        }
    }

    if (newFieldsCount) {
        auto oldCount = klass->field_count;

        auto newFields = (IL2CPP::FieldInfo *) BNM_malloc((oldCount + newFieldsCount) * sizeof(IL2CPP::FieldInfo));

        if (oldCount) memcpy(newFields, klass->fields, oldCount * sizeof(IL2CPP::FieldInfo));

        auto currentAddress = klass->instance_size;

        IL2CPP::FieldInfo *newField = newFields + oldCount;

        for (auto field : customClass->_fields) {
            SetupField(newField, field);
            newField->parent = klass;

            field->offset = (int32_t) currentAddress;
            newField->offset = (int32_t) currentAddress;
            field->myInfo = newField;

            currentAddress += field->_size;

            newField++;
            BNM_LOG_DEBUG("ModifyClass added field: %s", field->_name.data());
        }

        if ((klass->flags & BNM_CLASS_ALLOCATED_FIELDS_FLAG) == BNM_CLASS_ALLOCATED_FIELDS_FLAG) BNM_free(klass->fields);
        klass->flags |= (uint32_t) BNM_CLASS_ALLOCATED_FIELDS_FLAG;

        klass->actualSize = currentAddress;
        klass->fields = newFields;
        klass->field_count += newFieldsCount;
    }

    customClass->myClass = klass;
    customClass->type = Class(klass);
}

static char forEmptyString = '\0';
static const char *baseImageName = "Assembly-CSharp.dll";

static void CreateClass(MANAGEMENT_STRUCTURES::CustomClass *customClass, const CustomClassInfo &classInfo) {
    auto &api = Internal::api;
    Image image{};
    if (classInfo._imageName) {
        auto &assemblies = Internal::GetAllAssemblies();
        for (auto assembly : assemblies) {
            if (!assembly) continue;
            auto currentImage = api.il2cpp_assembly_get_image(assembly);
            if (!Internal::CompareImageName(currentImage, classInfo._imageName)) continue;
            image = currentImage;
            break;
        }
    } else image = Image(baseImageName);
    if (!image) image = MakeImage(classInfo._imageName);

    BNM_LOG_DEBUG("CreateClass target: %s.%s in %s", classInfo._namespace ? classInfo._namespace : &forEmptyString, classInfo._name, image._data->name);

    IL2CPP::Il2CppClass *parent = customClass->_baseType;
    if (!parent) parent = Internal::vmData.Object;

    IL2CPP::Il2CppClass *owner = customClass->_owner;

    std::vector<IL2CPP::Il2CppRuntimeInterfaceOffsetPair> newInterOffsets{};
    if (parent->interfaceOffsets)
        for (uint16_t i = 0; i < parent->interface_offsets_count; ++i)
            newInterOffsets.push_back(parent->interfaceOffsets[i]);

    auto allInterfaces = customClass->_interfaces;
    std::vector<IL2CPP::Il2CppClass *> interfaces{};
    for (auto &interface : allInterfaces)
        if (auto cls = interface.ToClass(); cls)
            GetAllInterfaces(parent, cls, interfaces);

    auto newVtableSize = parent->vtable_count;
    std::vector<IL2CPP::VirtualInvokeData> newVTable(newVtableSize);
    for (uint16_t i = 0; i < parent->vtable_count; ++i) newVTable[i] = parent->vtable[i];
    for (auto interface : interfaces) {
        newInterOffsets.push_back({interface, (int32_t) newVtableSize});
        void *iter = nullptr;
        while (auto v = api.il2cpp_class_get_methods(interface, &iter)) {
            newVtableSize++;
            newVTable.push_back({nullptr, (const IL2CPP::MethodInfo *) v});
        }
    }

    uint8_t hasFinalize = 0;
    auto methods = (const IL2CPP::MethodInfo **) BNM_malloc(customClass->_methods.size() * sizeof(IL2CPP::MethodInfo *));

    for (size_t i = 0; i < customClass->_methods.size(); ++i) {
        auto method = customClass->_methods[i];

        method->myInfo = ProcessCustomMethod(method, {});

        BNM_LOG_DEBUG("CreateClass added method: %s (%zu)", method->_name.data(), method->_parameterTypes.size());

        if (!method->_isStatic) for (uint16_t v = 0; v < newVtableSize; ++v) {
            auto &vTable = newVTable[v];
            if (!vTable.method) continue;
            auto count = vTable.method->parameters_count;

            if (!strcmp(vTable.method->name, method->myInfo->name) && count == method->myInfo->parameters_count && method->_parameterTypes.size() == count) {
                if (!method->_skipTypeMatch) {
                    bool typesMatch = true;
                    for (uint8_t p = 0; p < count; ++p) {
                        auto type = Internal::api.il2cpp_method_get_param((IL2CPP::MethodInfo *) vTable.method, p);
                        if (Class(type).GetClass() != method->_parameterTypes[p].ToIl2CppClass()) {
                            typesMatch = false;
                            break;
                        }
                    }
                    if (!typesMatch) continue;
                }

                if (!hasFinalize) hasFinalize = v == Internal::finalizerSlot;
                method->_origin = (IL2CPP::MethodInfo *) vTable.method;
                method->_originalAddress = (void *) (vTable.method ? vTable.method->methodPointer : nullptr);
                method->myInfo->slot = v;
                vTable.method = method->myInfo;
                vTable.methodPtr = method->myInfo->methodPointer;

                BNM_LOG_DEBUG("CreateClass overridden virtual: %s", MethodBase(method->_origin).str().c_str());

                break;
            }
        }

        methods[i] = method->myInfo;
    }

    auto klass = customClass->myClass = (IL2CPP::Il2CppClass *) BNM_malloc(sizeof(IL2CPP::Il2CppClass) + newVTable.size() * sizeof(IL2CPP::VirtualInvokeData));
    memset(klass, 0, sizeof(IL2CPP::Il2CppClass) + newVTable.size() * sizeof(IL2CPP::VirtualInvokeData));

    klass->image = image;

    auto len = strlen(classInfo._name);
    klass->name = (char *) BNM_malloc(len + 1);
    memcpy((void *) klass->name, classInfo._name, len);
    ((char *) klass->name)[len] = 0;

    if (!owner && classInfo._namespace) {
        len = strlen(classInfo._namespace);
        klass->namespaze = (char *) BNM_malloc(len + 1);
        memcpy((void *) klass->namespaze, classInfo._namespace, len);
        ((char *) klass->namespaze)[len] = 0;
    } else klass->namespaze = &forEmptyString;

    SetupTypes(klass);

    klass->element_class = klass;
    klass->castClass = klass;

    SetupClassParent(klass, parent);
    SetupClassOwner(klass, owner);

    klass->generic_class = nullptr;
    klass->typeMetadataHandle = nullptr;

    klass->klass = klass;

    klass->field_count = customClass->_fields.size();
    if (klass->field_count > 0) {
        auto fields = (IL2CPP::FieldInfo *) BNM_malloc(klass->field_count * sizeof(IL2CPP::FieldInfo));

        IL2CPP::FieldInfo *newField = fields;
        for (auto field : customClass->_fields) {
            SetupField(newField, field);
            newField->parent = klass;

            newField->offset = (int32_t) field->offset;
            field->myInfo = newField;

            newField++;
        }
        klass->fields = fields;
    } else klass->fields = nullptr;

    if (!interfaces.empty()) {
        klass->interfaces_count = interfaces.size();
        klass->implementedInterfaces = (IL2CPP::Il2CppClass **) BNM_malloc(interfaces.size() * sizeof(IL2CPP::Il2CppClass *));
        memcpy(klass->implementedInterfaces, interfaces.data(), interfaces.size() * sizeof(IL2CPP::Il2CppClass *));
    } else {
        klass->interfaces_count = 0;
        klass->implementedInterfaces = nullptr;
    }

    for (auto method : customClass->_methods) PRIVATE_INTERNAL::GetMethodClass(method->myInfo) = klass;
    klass->method_count = customClass->_methods.size();
    klass->methods = methods;
    klass->has_finalize = hasFinalize;

    klass->flags = (klass->parent->flags & ~(0x00000080 | 0x00000020)) | 0x0000001;

    klass->native_size = -1;
    klass->element_size = 0;
    klass->instance_size = klass->actualSize = customClass->_size;

    klass->vtable_count = newVTable.size();
    for (size_t i = 0; i < newVTable.size(); ++i) klass->vtable[i] = newVTable[i];

    klass->interface_offsets_count = newInterOffsets.size();
    klass->interfaceOffsets = (IL2CPP::Il2CppRuntimeInterfaceOffsetPair *) BNM_malloc(newInterOffsets.size() * sizeof(IL2CPP::Il2CppRuntimeInterfaceOffsetPair));
    memcpy(klass->interfaceOffsets, newInterOffsets.data(), newInterOffsets.size() * sizeof(IL2CPP::Il2CppRuntimeInterfaceOffsetPair));

    klass->interopData = nullptr;
    klass->events = nullptr;
    klass->properties = nullptr;
    klass->nestedTypes = nullptr;
    klass->rgctx_data = nullptr;

    klass->static_fields = nullptr;
    klass->static_fields_size = 0;

    klass->genericRecursionDepth = 0;
    klass->genericContainerHandle = nullptr;

    klass->stack_slot_size = sizeof(void *);

    klass->has_cctor = 0;
    klass->has_references = 1;
    klass->size_inited = 1;
    klass->is_vtable_initialized = 1;

    klass->enumtype = 0;
    klass->token = 0;
    klass->minimumAlignment = 1;
    klass->is_generic = 0;
    klass->rank = 0;
    klass->nested_type_count = 0;
    klass->thread_static_fields_offset = 0;
    klass->thread_static_fields_size = 0;
    klass->cctor_started = 0;
    klass->packingSize = 0;

    klass->size_init_pending = 0;

    klass->cctor_finished_or_no_cctor = 1;
    klass->nullabletype = 0;

    klass->cctor_thread = 0;

    klass->initialized = 0;
    klass->init_pending = 0;
    klass->initialized_and_no_error = 0;
    klass->initializationExceptionGCHandle = 0;

    klass->gc_desc = nullptr;

    api.il2cpp_runtime_class_init(klass);

    AddClassToMap(image.GetInfo(), klass);

    customClass->type = Class(klass);
}

static CustomClassInfo GetClassInfo(const CompileTimeClass &compileTimeClass) {
    CompileTimeClass tmp{};

    auto &stack = compileTimeClass._stack;

    for (auto info : stack) {
        if (info->_baseType != CompileTimeClass::_BaseType::Class) continue;

        auto classInfo = (CompileTimeClass::_ClassInfo *) info;

        auto ns = classInfo->_namespace ? classInfo->_namespace : "";
        auto name = classInfo->_name;

        auto it = std::find(stack.begin(), stack.end(), info);
        bool isLastClass = true;
        for (auto jt = it + 1; jt != stack.end(); ++jt) {
            if ((*jt)->_baseType == CompileTimeClass::_BaseType::Class) {
                isLastClass = false;
                break;
            }
        }

        auto resolved = Internal::TryGetClassWithoutImage(ns, name);
        if (resolved) continue;

        if (isLastClass) return {ns, name, classInfo->_imageName};
        return {};
    }
    return {._class = compileTimeClass.ToClass()};
}

static IL2CPP::Il2CppImage *MakeImage(std::string_view imageName) {
    auto newImg = BNM_I2C_NEW(Il2CppImage);
    memset(newImg, 0, sizeof(IL2CPP::Il2CppImage));

    if (imageName.ends_with(".dll")) imageName.remove_suffix(4);

    auto nameLen = imageName.size();
    newImg->nameNoExt = (char *) BNM_malloc(nameLen + 1);
    memcpy((void *) newImg->nameNoExt, (void *) imageName.data(), nameLen);
    ((char *) newImg->nameNoExt)[nameLen] = 0;

    newImg->name = (char *) BNM_malloc(nameLen + 5);
    memcpy((void *) newImg->name, (void *) imageName.data(), nameLen);
    auto nameEnd = ((char *) (newImg->name + nameLen));
    nameEnd[0] = '.';
    nameEnd[1] = 'd';
    nameEnd[2] = 'l';
    nameEnd[3] = 'l';
    nameEnd[4] = 0;

    newImg->assembly = nullptr;
    newImg->customAttributeCount = 0;
    newImg->typeCount = 0;
    newImg->exportedTypeCount = 0;
    newImg->token = 1;

    auto newAsm = BNM_I2C_NEW(Il2CppAssembly);
    memset(newAsm, 0, sizeof(IL2CPP::Il2CppAssembly));
    newAsm->image = newImg;
    newImg->assembly = newAsm;
    auto &aName = newAsm->aname;
    aName.name = newImg->name;
    aName.culture = nullptr;
    aName.public_key = nullptr;
    aName.public_key_token[0] = 0;
    aName.hash_len = 0;
    aName.hash_alg = 0;
    aName.flags = 0;
    newAsm->referencedAssemblyStart = -1;
    newAsm->referencedAssemblyCount = 0;

    newImg->nameToClassHashTable = (decltype(newImg->nameToClassHashTable)) - 0x424e4d;

    Internal::assembliesCache.push_back(newAsm);

    BNM_LOG_INFO("MakeImage added: %s", imageName.data());

    return newImg;
}

static IL2CPP::VirtualInvokeData *TryFindVirtualMethod(Class target, IL2CPP::MethodInfo *targetMethod, uint16_t *outSlot = nullptr) {
    for (uint16_t i = 0; i < target._data->vtable_count; ++i) {
        auto &it = target._data->vtable[i];
        if (it.method != targetMethod) continue;
        if (outSlot) *outSlot = i;
        return &it;
    }
    if (outSlot) *outSlot = 65535;
    return nullptr;
}

static IL2CPP::MethodInfo *ProcessCustomMethod(MANAGEMENT_STRUCTURES::CustomMethod *method, Class target, bool *hooked) {
    if (!target) return CreateMethod(method);

    struct _ClearMethod {
        MANAGEMENT_STRUCTURES::CustomMethod *method;
        ~_ClearMethod() {
            method->_parameterTypes.clear();
            method->_parameterTypes.shrink_to_fit();
        }
    } _clearMethod{method};

    auto parameters = (uint8_t) method->_parameterTypes.size();

    auto originalMethod = Internal::IterateMethods(target, [method, parameters](IL2CPP::MethodInfo *klassMethod) {
        if (method->_name != (klassMethod->name ? klassMethod->name : "") || klassMethod->parameters_count != parameters) return false;
        if (method->_skipTypeMatch) return true;

        for (uint8_t i = 0; i < parameters; ++i) {
            auto param = Internal::api.il2cpp_method_get_param(klassMethod, i);
            if (Class(param).GetClass() != method->_parameterTypes[i].ToIl2CppClass()) return false;
        }
        return true;
    });

    if (!originalMethod || PRIVATE_INTERNAL::GetMethodClass(originalMethod) != target._data) {
        bool isVirtual = originalMethod != nullptr && (originalMethod->flags & 0x0040) == 0x0040;
        auto parent = originalMethod;
        originalMethod = CreateMethod(method);

        if (!method->_copyTarget.empty()) {
            auto copyTarget = Internal::IterateMethods(target, [targetName = method->_copyTarget](IL2CPP::MethodInfo *method) { return targetName == (method->name ? method->name : ""); });
            if (copyTarget) originalMethod->token = copyTarget->token;
        }

        bool hasNonVirtualParent = !isVirtual && parent;

        if (hasNonVirtualParent) {
            method->_origin = parent;
            method->_originalAddress = (void *) parent->methodPointer;
            return originalMethod;
        }

        bool canDoVirtualHook = hooked && isVirtual;

        if (!canDoVirtualHook) return originalMethod;

        uint16_t slot{};
        if (auto vTable = TryFindVirtualMethod(target, parent, &slot); vTable != nullptr) {
            method->_origin = parent;
            method->_originalAddress = (void *) parent->methodPointer;
            originalMethod->slot = slot;
            vTable->methodPtr = (void (*)()) method->_address;
            vTable->method = originalMethod;
            return originalMethod;
        }
    }

    if (method->_isInvokeHook) {
        method->_origin = originalMethod;
        method->_originalAddress = (void *) originalMethod->methodPointer;
        originalMethod->methodPointer = (void (*)()) method->_address;
        if (hooked) *hooked = true;
        return originalMethod;
    }

    if ((originalMethod->flags & 0x0040) == 0 || method->_isBasicHook) goto SKIP_NON_VIRTUAL;

    if (auto vTable = TryFindVirtualMethod(target, originalMethod); vTable != nullptr) {
        method->_origin = (IL2CPP::MethodInfo *) vTable->method;
        method->_originalAddress = (void *) method->_origin->methodPointer;
        vTable->methodPtr = (void (*)()) method->_address;
        if (hooked) *hooked = true;
        return originalMethod;
    }

    SKIP_NON_VIRTUAL:

    method->_origin = originalMethod;
    ::BasicHook((void *) originalMethod->methodPointer, method->_address, method->_originalAddress);

    if (hooked) *hooked = true;

    return originalMethod;
}

static IL2CPP::MethodInfo *CreateMethod(MANAGEMENT_STRUCTURES::CustomMethod *method) {
    auto *myInfo = BNM_I2C_NEW(MethodInfo);
    memset(myInfo, 0, sizeof(IL2CPP::MethodInfo));
    myInfo->methodPointer = (decltype(myInfo->methodPointer)) method->_address;
    myInfo->invoker_method = (decltype(myInfo->invoker_method)) method->_invoker;
    myInfo->parameters_count = method->_parameterTypes.size();

    myInfo->virtualMethodPointer = (decltype(myInfo->virtualMethodPointer)) method->_address;

    auto name = (char *) BNM_malloc(method->_name.size() + 1);
    memcpy((void *) name, method->_name.data(), method->_name.size());
    name[method->_name.size()] = 0;
    myInfo->name = name;

    myInfo->flags = 0x0006 | 0x0080;
    if (method->_isStatic) myInfo->flags |= 0x0010;
    if (method->_name == Internal::constructorName) myInfo->flags |= 0x0800 | 0x1000;

    myInfo->is_generic = false;
    myInfo->is_inflated = false;

    myInfo->slot = 65535;

    myInfo->token = 0;

    auto methodType = method->_returnType.ToClass();
    if (!methodType) methodType = Internal::vmData.Object;
    myInfo->return_type = methodType.GetIl2CppType();

    auto argsCount = myInfo->parameters_count;
    if (argsCount) {
        auto &types = method->_parameterTypes;
        auto parameters = (IL2CPP::Il2CppType **) BNM_malloc(argsCount * sizeof(IL2CPP::Il2CppType *));

        myInfo->parameters = (const IL2CPP::Il2CppType **) parameters;
        for (uint8_t p = 0; p < argsCount; ++p) {
            auto parameter = BNM_I2C_NEW(Il2CppType);

            auto type = p < types.size() ? types[p].ToClass() : Internal::vmData.Object;
            if (!type) type = Internal::vmData.Object;
            *parameter = *type.GetIl2CppType();

            parameters[p] = parameter;
        }
    }
    myInfo->rgctx_data = nullptr;
    myInfo->genericMethod = nullptr;

    method->_parameterTypes.clear();
    method->_parameterTypes.shrink_to_fit();

    return myInfo;
}

static void SetupField(IL2CPP::FieldInfo *newField, MANAGEMENT_STRUCTURES::CustomField *field) {
    auto name = field->_name;
    auto len = name.size();
    newField->name = (char *) BNM_malloc(len + 1);
    memcpy((void *) newField->name, name.data(), len);
    ((char *) newField->name)[len] = 0;

    newField->type = BNM_I2C_NEW(Il2CppType);
    auto fieldType = field->_type.ToClass();
    if (!fieldType) fieldType = Internal::vmData.Object;

    auto newType = (IL2CPP::Il2CppType *) newField->type;
    *newType = *fieldType.GetIl2CppType();
    newType->attrs = newField->token = 0x0006;
}

static void SetupClassOwner(IL2CPP::Il2CppClass *target, IL2CPP::Il2CppClass *owner) {
    if (!owner) return;

    auto oldOwner = target->declaringType;
    auto oldInnerList = owner->nestedTypes;

    target->declaringType = owner;

    auto newInnerList = (IL2CPP::Il2CppClass **) BNM_malloc(sizeof(IL2CPP::Il2CppClass) * (owner->nested_type_count + 1));
    if (owner->nested_type_count) memcpy(newInnerList, owner->nestedTypes, sizeof(IL2CPP::Il2CppClass) * owner->nested_type_count);
    newInnerList[owner->nested_type_count++] = target;
    owner->nestedTypes = newInnerList;

    if ((owner->flags & BNM_CLASS_ALLOCATED_INNER_FLAG) == BNM_CLASS_ALLOCATED_INNER_FLAG) BNM_free(oldInnerList);
    owner->flags |= (uint32_t) BNM_CLASS_ALLOCATED_INNER_FLAG;

    if (oldOwner) {
        oldInnerList = oldOwner->nestedTypes;
        newInnerList = (IL2CPP::Il2CppClass **) BNM_malloc(sizeof(IL2CPP::Il2CppClass) * (oldOwner->nested_type_count - 1));
        uint8_t skipped = 0;
        for (uint16_t i = 0; i < oldOwner->nested_type_count; ++i) {
            if (skipped == 0) if (skipped = (oldInnerList[i] == target); skipped) continue;
            newInnerList[i - skipped] = oldInnerList[i];
        }
        oldOwner->nestedTypes = newInnerList;
        --oldOwner->nested_type_count;

        if ((oldOwner->flags & BNM_CLASS_ALLOCATED_INNER_FLAG) == BNM_CLASS_ALLOCATED_INNER_FLAG) BNM_free(oldInnerList);
        oldOwner->flags |= (uint32_t) BNM_CLASS_ALLOCATED_INNER_FLAG;
    }
}

static void SetupClassParent(IL2CPP::Il2CppClass *target, IL2CPP::Il2CppClass *parent) {
    if (!parent) [[unlikely]] return;

    if ((target->flags & BNM_CLASS_ALLOCATED_HIERARCHY_FLAG) == BNM_CLASS_ALLOCATED_HIERARCHY_FLAG) BNM_free(target->typeHierarchy);
    target->flags |= (uint32_t) BNM_CLASS_ALLOCATED_HIERARCHY_FLAG;

    target->typeHierarchyDepth = parent->typeHierarchyDepth + 1;
    target->typeHierarchy = (IL2CPP::Il2CppClass **) BNM_malloc(target->typeHierarchyDepth * sizeof(IL2CPP::Il2CppClass *));
    if (parent->typeHierarchyDepth) memcpy(target->typeHierarchy, parent->typeHierarchy, parent->typeHierarchyDepth * sizeof(IL2CPP::Il2CppClass *));
    target->typeHierarchy[parent->typeHierarchyDepth] = target;
    target->parent = parent;
}

static void GetAllInterfaces(IL2CPP::Il2CppClass *parent, IL2CPP::Il2CppClass *interface, std::vector<IL2CPP::Il2CppClass *> &outInterfaces) {
    Internal::api.il2cpp_runtime_class_init(interface);
    if (!HasInterface(parent, interface)) outInterfaces.push_back(interface);
    if (!interface->interfaces_count || interface->interfaces_count == (uint16_t) - 1) return;

    for (uint16_t i = 0; i < interface->interfaces_count; ++i) GetAllInterfaces(parent, interface->implementedInterfaces[i], outInterfaces);
}

static bool HasInterface(IL2CPP::Il2CppClass *parent, IL2CPP::Il2CppClass *interface) {
    if (!parent || !interface) return false;
    for (uint16_t i = 0; i < parent->interfaces_count; ++i) if (parent->implementedInterfaces[i] == interface) return true;
    if (parent->parent) return HasInterface(parent->parent, interface);
    return false;
}

static void SetupTypes(IL2CPP::Il2CppClass *target) {
    IL2CPP::Il2CppType classType;
    memset(&classType, 0, sizeof(IL2CPP::Il2CppType));
    classType.type = IL2CPP::IL2CPP_TYPE_CLASS;
    classType.attrs = 0x0;
    classType.pinned = 0;
    classType.byref = 0;
    classType.num_mods = 31;
    classType.data.dummy = target;

    target->this_arg = target->byval_arg = classType;
    target->this_arg.byref = 1;
}

IL2CPP::Il2CppClass *(*old_Class$$FromIl2CppType)(IL2CPP::Il2CppType *type){};
IL2CPP::Il2CppClass *(*old_Type$$GetClassOrElementClass)(IL2CPP::Il2CppType *type){};
IL2CPP::Il2CppClass *(*old_Class$$FromName)(IL2CPP::Il2CppImage *image, const char *namespace_, const char *name){};

IL2CPP::Il2CppClass *Class$$FromIl2CppType(IL2CPP::Il2CppType *type) {
    if (!type) return nullptr;
    if (type->num_mods == 31) return (IL2CPP::Il2CppClass *) type->data.dummy;
    return old_Class$$FromIl2CppType ? old_Class$$FromIl2CppType(type) : nullptr;
}

IL2CPP::Il2CppClass *Type$$GetClassOrElementClass(IL2CPP::Il2CppType *type) {
    if (!type) return nullptr;
    if (type->num_mods == 31) return (IL2CPP::Il2CppClass *) type->data.dummy;
    return old_Type$$GetClassOrElementClass ? old_Type$$GetClassOrElementClass(type) : nullptr;
}

IL2CPP::Il2CppClass *Class$$FromName(IL2CPP::Il2CppImage *image, const char *namespace_, const char *name) {
    auto &api = Internal::api;
    if (!image) return nullptr;

    IL2CPP::Il2CppClass *ret = nullptr;

    if (image->nameToClassHashTable != (decltype(image->nameToClassHashTable)) - 0x424e4d)
        ret = old_Class$$FromName ? old_Class$$FromName(image, namespace_, name) : api.il2cpp_class_from_name(image, namespace_, name);

    if (!ret) ForEachClassInImageImpl(image, [namespace_, name, &ret](IL2CPP::Il2CppClass *cls) -> bool {
        auto ns = api.il2cpp_class_get_namespace(cls);
        auto clsName = api.il2cpp_class_get_name(cls);
        if (!strcmp(namespace_, ns ? ns : "") && !strcmp(name, clsName ? clsName : "")) {
            ret = cls;
            return true;
        }
        return false;
    });

    return ret;
}

}

namespace BNM::Internal {

void (*orig_Image$$GetTypes)(const IL2CPP::Il2CppImage *image, bool exportedOnly, std::vector<BNM::IL2CPP::Il2CppClass *> *target){};

void Image$$GetTypes(const IL2CPP::Il2CppImage *image, bool, std::vector<BNM::IL2CPP::Il2CppClass *> *target) {
    if (!image || !target) return;
    if (image->nameToClassHashTable != (decltype(image->nameToClassHashTable)) - 0x424e4d && orig_Image$$GetTypes)
        orig_Image$$GetTypes(image, false, target);
    auto it = ClassesManagement::bnmClassesMap.find((BNM_PTR) image);
    if (it == ClassesManagement::bnmClassesMap.end()) return;
    for (auto cls : it->second) target->push_back(cls);
}
}
