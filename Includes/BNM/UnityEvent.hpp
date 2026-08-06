#pragma once

#include "Class.hpp"
#include "MonoStructures.hpp"
namespace BNM {
namespace UnityEngine {
    inline BNM::Class GetTypeByName(const std::string_view &name) {
        return Class("UnityEngine", name, Image("UnityEngine.CoreModule.dll"));
    }
    inline BNM::Class GetTypeByName(const std::string_view &module, const std::string_view &name) {
        return Class("UnityEngine", name, Image(module));
    }
    inline BNM::Class GetType(const std::string_view &name) { return GetTypeByName(name); }
}

namespace UnityEngine {

struct PersistentCall;

struct UnityEventBase {
    IL2CPP::Il2CppObject object;
    static BNM::Class GetArgumentType(PersistentCall *call);
    static BNM::Class GetTargetType(PersistentCall *call);
};

struct PersistentCallArguments {
    IL2CPP::Il2CppObject object;
    Structures::Mono::String *m_ObjectArgumentAssemblyTypeName{};
    void *m_ObjectArgument{};
    int32_t m_Mode{};
};

struct PersistentCall {
    BNM::Class m_Target{};
    Structures::Mono::String *m_TargetAssemblyTypeName{};
    PersistentCallArguments *m_Arguments{};
    IL2CPP::MethodInfo *m_Method{};
};

}
}
