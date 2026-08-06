#include "BNM.hpp"

using namespace BNM;

Structures::Mono::String *BNM::CreateMonoString(const std::string_view &str) {
    return (Structures::Mono::String *) Internal::api.il2cpp_string_new(str.data());
}

namespace BNM::Structures::Mono::PRIVATE_MonoListData {
    static std::map<uint32_t, IL2CPP::Il2CppClass *> customListsMap{};

    void *CompareExchange4List(void *syncRoot) {
        if (Internal::vmData.Interlocked$$CompareExchange.IsValid()) {
            auto m = Internal::vmData.Interlocked$$CompareExchange.cast<Method<void *>>();
            m.SetInstance(nullptr);
            m((void **) &syncRoot, (void *) Internal::vmData.Object.CreateNewInstance(), (void *) nullptr);
        }
        return syncRoot;
    }

    IL2CPP::Il2CppClass *TryGetMonoListClass(uint32_t typeHash, MethodData *data, size_t count) {
        auto &klass = customListsMap[typeHash];
        if (klass) return klass;

        auto templateClass = Internal::customListTemplateClass;
        if (!templateClass) return nullptr;
        auto size = sizeof(IL2CPP::Il2CppClass) + templateClass->vtable_count * sizeof(IL2CPP::VirtualInvokeData);
        auto typedClass = (IL2CPP::Il2CppClass *) BNM_malloc(size);
        memcpy(typedClass, templateClass, size);

        std::map<size_t, IL2CPP::MethodInfo *> createdMethods{};
        for (uint16_t i = 4; i < typedClass->vtable_count; ++i) {
            auto &cur = typedClass->vtable[i];
            if (!cur.method || !cur.method->name) continue;
            auto name = std::string_view(cur.method->name);
            auto dot = name.rfind('.');
            if (dot != std::string_view::npos) name = name.substr(dot + 1);

            auto iterator = data;
            size_t c = 0;
            for (; c < count; ++c, ++iterator) if (iterator->name && name == iterator->name) break;
            if (c == count) continue;

            auto &methodInfo = createdMethods[FNV1a(name)];
            if (!methodInfo) {
                methodInfo = (IL2CPP::MethodInfo *) BNM_malloc(sizeof(IL2CPP::MethodInfo));
                *methodInfo = *cur.method;
                methodInfo->methodPointer = (IL2CPP::Il2CppMethodPointer) iterator->ptr;
            }
            cur.method = methodInfo;
            cur.methodPtr = methodInfo->methodPointer;
        }
        klass = typedClass;
        return klass;
    }
}

namespace BNM::Exceptions {
    Exception TryInvoke(const std::function<void()> &func) {
        auto &api = BNM::Internal::api;
        IL2CPP::Il2CppType type;
        memset(&type, 0, sizeof(type));
        type.type = IL2CPP::IL2CPP_TYPE_VOID;
        IL2CPP::Il2CppClass klass;
        memset(&klass, 0, sizeof(klass));
        IL2CPP::MethodInfo info;
        memset(&info, 0, sizeof(info));
        info.return_type = &type;
        PRIVATE_INTERNAL::GetMethodClass(&info) = &klass;
        info.methodPointer = (decltype(info.methodPointer)) &func;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
        info.invoker_method = (IL2CPP::InvokerMethod) +[](std::function<void()> *f) -> void { (*f)(); };
#pragma GCC diagnostic pop
        IL2CPP::Il2CppException *exception = nullptr;
        api.il2cpp_runtime_invoke(&info, nullptr, nullptr, &exception);
        return {exception};
    }
}

Structures::Mono::String *Structures::Mono::String::Empty() {
    return (Structures::Mono::String *) (Internal::vmData.String$$Empty ? *Internal::vmData.String$$Empty : nullptr);
}

template<typename T>
Structures::Mono::Array<T> *Structures::Mono::Array<T>::Create(IL2CPP::il2cpp_array_size_t size, bool zeroed) {
    auto cls = BNM::Defaults::Get<T>().ToClass();
    if (!cls) return nullptr;
    auto arrayClass = cls.GetArray();
    if (!arrayClass) return nullptr;
    auto arr = (Array<T> *) BNM::Internal::api.il2cpp_array_new(arrayClass._data, size);
    if (arr && zeroed) memset((char *) arr + sizeof(IL2CPP::Il2CppArray), 0, size * sizeof(T));
    return arr;
}

template Structures::Mono::Array<MonoType *> *Structures::Mono::Array<MonoType *>::Create(IL2CPP::il2cpp_array_size_t, bool);
template Structures::Mono::Array<int> *Structures::Mono::Array<int>::Create(IL2CPP::il2cpp_array_size_t, bool);
