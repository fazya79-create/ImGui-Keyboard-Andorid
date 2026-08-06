#pragma once

#include "Debug.hpp"
#include "Class.hpp"
#include "Internals.hpp"
#include "CompileTimeClass.hpp"
namespace BNM {
extern bool InvertMatrix4x4_Full(const float *inMatrix, float *dest);

struct CustomWait : BNM::IL2CPP::Il2CppObject {
    std::function<bool()> _func{};
    bool _isUntil = false;
    void Finalize() {
        try {
            _func = {};
            this->~CustomWait();
        } catch (...) {
            BNM_LOG_ERR("CustomWait::Finalize exception");
        }
    }
    bool MoveNext() const {
        try {
            if (!_func) return false;
            return _isUntil == !_func();
        } catch (...) {
            BNM_LOG_ERR("CustomWait::MoveNext exception");
            return false;
        }
    }
    void Reset() {}
    static IL2CPP::Il2CppObject *Current() { return nullptr; }
};

namespace Coroutine {

struct YieldInstruction {
    inline YieldInstruction(BNM::IL2CPP::Il2CppObject *object) : _object(object) {}
    BNM::IL2CPP::Il2CppObject *_object{};
protected:
    inline YieldInstruction() = default;
    friend struct IEnumerator;
};

struct IEnumerator : BNM::IL2CPP::Il2CppObject {
    struct promise_type {
        Coroutine::YieldInstruction _currentValue{};
        inline IEnumerator get_return_object() { return IEnumerator(std::coroutine_handle<promise_type>::from_promise(*this)); }
        inline std::suspend_always initial_suspend() noexcept { return {}; }
        inline std::suspend_always final_suspend() noexcept { return {}; }
        inline void unhandled_exception() {}
        inline std::suspend_always await_transform() = delete;
        [[nodiscard]] inline Coroutine::YieldInstruction value() const noexcept { return _currentValue; }
        inline std::suspend_always yield_value(const Coroutine::YieldInstruction &val) { _currentValue = val; return {}; }
        inline void return_void() {}
    };
    IL2CPP::Il2CppObject *_current{};
    std::coroutine_handle<promise_type> _coroutine{};

    void Finalize();
    bool MoveNext();
    void Reset();
    IL2CPP::Il2CppObject *Current();
    IEnumerator *Get();
    inline IEnumerator *operator()() { return Get(); }
    inline operator IEnumerator *() { return Get(); }
    explicit IEnumerator(std::coroutine_handle<promise_type> handle) : BNM::IL2CPP::Il2CppObject(), _coroutine(handle) {}
    inline constexpr IEnumerator() : BNM::IL2CPP::Il2CppObject() {}
};

struct AsyncOperation : YieldInstruction {
    inline AsyncOperation() = default;
    AsyncOperation(intptr_t operation);
};

struct WaitForEndOfFrame : YieldInstruction {
    WaitForEndOfFrame();
};

struct WaitForFixedUpdate : YieldInstruction {
    WaitForFixedUpdate();
};

struct WaitForSeconds : YieldInstruction {
    inline WaitForSeconds() = default;
    WaitForSeconds(float seconds);
};

struct WaitForSecondsRealtime : YieldInstruction {
    inline WaitForSecondsRealtime() = default;
    WaitForSecondsRealtime(float seconds);
};

struct WaitUntil : YieldInstruction {
    inline WaitUntil() = default;
    WaitUntil(const std::function<bool()> &function);
};

struct WaitWhile : YieldInstruction {
    inline WaitWhile() = default;
    WaitWhile(const std::function<bool()> &function);
};

}

struct BNMClassBuilder {
    static inline CompileTimeClass Class(const char *_namespace, const char *name, const char *imageName = nullptr) {
        return CompileTimeClassBuilder(_namespace, name, imageName).Build();
    }
};

bool InvokeHookImpl(IL2CPP::MethodInfo *info, void *newMet, void **oldMet);
bool VirtualHookImpl(Class targetClass, IL2CPP::MethodInfo *info, void *newMet, void **oldMet);

template<typename T_NEW, typename T_OLD>
inline bool InvokeHook(const BNM::MethodBase &targetMethod, T_NEW newMet, T_OLD &oldMet) {
    return InvokeHookImpl(targetMethod.GetInfo(), (void *) newMet, (void **) &oldMet);
}
template<typename T_NEW, typename T_OLD>
inline bool InvokeHook(const BNM::MethodBase &targetMethod, T_NEW newMet, T_OLD &&oldMet) {
    return InvokeHookImpl(targetMethod.GetInfo(), (void *) newMet, (void **) &oldMet);
}
template<typename T_NEW, typename T_OLD>
inline bool InvokeHook(IL2CPP::MethodInfo *info, T_NEW newMet, T_OLD &oldMet) {
    return InvokeHookImpl(info, (void *) newMet, (void **) &oldMet);
}
template<typename T_NEW, typename T_OLD>
inline bool InvokeHook(IL2CPP::MethodInfo *info, T_NEW newMet, T_OLD &&oldMet) {
    return InvokeHookImpl(info, (void *) newMet, (void **) &oldMet);
}
}
