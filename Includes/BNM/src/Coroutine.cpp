#include "BNM.hpp"

using namespace BNM;

namespace BNM::Internal {
MANAGEMENT_STRUCTURES::CustomClass coroutineIEClass{};
MANAGEMENT_STRUCTURES::CustomClass coroutineWaitClass{};
BNM::Class coroutineAsyncOperation{}, coroutineWaitForEndOfFrame{}, coroutineWaitForFixedUpdate{}, coroutineWaitForSeconds{}, coroutineWaitForSecondsRealtime{};

}

void BNM::Internal::SetupCoroutine() {
    using namespace BNM::MANAGEMENT_STRUCTURES;

    static CustomMethod ieMoveNext{}, ieReset{}, ieCurrent{};
    coroutineIEClass._size = sizeof(BNM::Coroutine::IEnumerator);
    coroutineIEClass._targetType = BNM::CompileTimeClassBuilder("BNM.Coroutine", "IEnumerator").Build();
    coroutineIEClass._baseType = {};
    coroutineIEClass._owner = {};
    coroutineIEClass._interfaces = {BNM::CompileTimeClassBuilder("System.Collections", "IEnumerator", "mscorlib.dll").Build()};
    AddClass(&coroutineIEClass);

    {
        constexpr auto p = &BNM::Coroutine::IEnumerator::MoveNext;
        ieMoveNext._address = *(void **) &p;
        ieMoveNext._invoker = (void *) &GetMethodInvoker<false, decltype(&BNM::Coroutine::IEnumerator::MoveNext)>::Invoke;
        ieMoveNext._name = "MoveNext";
        ieMoveNext._returnType = BNM::Defaults::Get<bool>();
        ieMoveNext._isStatic = false;
        coroutineIEClass._methods.push_back(&ieMoveNext);
    }
    {
        constexpr auto p = &BNM::Coroutine::IEnumerator::Reset;
        ieReset._address = *(void **) &p;
        ieReset._invoker = (void *) &GetMethodInvoker<false, decltype(&BNM::Coroutine::IEnumerator::Reset)>::Invoke;
        ieReset._name = "Reset";
        ieReset._returnType = BNM::Defaults::Get<void>();
        ieReset._isStatic = false;
        coroutineIEClass._methods.push_back(&ieReset);
    }
    {
        constexpr auto p = &BNM::Coroutine::IEnumerator::Current;
        ieCurrent._address = *(void **) &p;
        ieCurrent._invoker = (void *) &GetMethodInvoker<false, decltype(&BNM::Coroutine::IEnumerator::Current)>::Invoke;
        ieCurrent._name = "get_Current";
        ieCurrent._returnType = BNM::Defaults::Get<BNM::IL2CPP::Il2CppObject *>();
        ieCurrent._isStatic = false;
        coroutineIEClass._methods.push_back(&ieCurrent);
    }

    static CustomMethod cwMoveNext{}, cwReset{}, cwCurrent{};
    coroutineWaitClass._size = sizeof(CustomWait);
    coroutineWaitClass._targetType = BNM::CompileTimeClassBuilder("BNM.Coroutine", "CustomWait").Build();
    coroutineWaitClass._baseType = {};
    coroutineWaitClass._owner = {};
    coroutineWaitClass._interfaces = {BNM::CompileTimeClassBuilder("System.Collections", "IEnumerator", "mscorlib.dll").Build()};
    AddClass(&coroutineWaitClass);

    {
        constexpr auto p = &CustomWait::MoveNext;
        cwMoveNext._address = *(void **) &p;
        cwMoveNext._invoker = (void *) &GetMethodInvoker<false, decltype(&CustomWait::MoveNext)>::Invoke;
        cwMoveNext._name = "MoveNext";
        cwMoveNext._returnType = BNM::Defaults::Get<bool>();
        cwMoveNext._isStatic = false;
        coroutineWaitClass._methods.push_back(&cwMoveNext);
    }
    {
        constexpr auto p = &CustomWait::Reset;
        cwReset._address = *(void **) &p;
        cwReset._invoker = (void *) &GetMethodInvoker<false, decltype(&CustomWait::Reset)>::Invoke;
        cwReset._name = "Reset";
        cwReset._returnType = BNM::Defaults::Get<void>();
        cwReset._isStatic = false;
        coroutineWaitClass._methods.push_back(&cwReset);
    }
    {
        constexpr auto p = &CustomWait::Current;
        cwCurrent._address = *(void **) &p;
        cwCurrent._invoker = (void *) &GetMethodInvoker<true, decltype(&CustomWait::Current)>::Invoke;
        cwCurrent._name = "get_Current";
        cwCurrent._returnType = BNM::Defaults::Get<BNM::IL2CPP::Il2CppObject *>();
        cwCurrent._isStatic = false;
        coroutineWaitClass._methods.push_back(&cwCurrent);
    }
}

void BNM::Internal::LoadCoroutine() {
    auto image = BNM::Image("UnityEngine.CoreModule.dll");
    auto _namespace = "UnityEngine";

    coroutineAsyncOperation = BNM::Class(_namespace, "AsyncOperation", image);
    coroutineWaitForEndOfFrame = BNM::Class(_namespace, "WaitForEndOfFrame", image);
    coroutineWaitForFixedUpdate = BNM::Class(_namespace, "WaitForFixedUpdate", image);
    coroutineWaitForSeconds = BNM::Class(_namespace, "WaitForSeconds", image);
    coroutineWaitForSecondsRealtime = BNM::Class(_namespace, "WaitForSecondsRealtime", image);
}

void BNM::Coroutine::IEnumerator::Finalize() {
    try {
        if (_coroutine) {
            _coroutine.destroy();
            _coroutine = nullptr;
        }
        this->~IEnumerator();
    } catch (...) {
        BNM_LOG_ERR("IEnumerator::Finalize exception: %s", Internal::GetExceptionTypeName());
    }
}

bool BNM::Coroutine::IEnumerator::MoveNext() {
    try {
        if (!_coroutine) return false;
        _coroutine.resume();
        if (_coroutine.done()) {
            _coroutine.destroy();
            _coroutine = nullptr;
            return false;
        }
        _current = _coroutine.promise().value()._object;
        return true;
    } catch (...) {
        if (_coroutine) {
            _coroutine.destroy();
            _coroutine = nullptr;
        }
        BNM_LOG_ERR("IEnumerator::MoveNext exception: %s", Internal::GetExceptionTypeName());
        return false;
    }
}

BNM::Coroutine::IEnumerator *BNM::Coroutine::IEnumerator::Get() {
    try {
        auto inst = (BNM::Coroutine::IEnumerator *) BNM::Class(Internal::coroutineIEClass.myClass).CreateNewInstance();
        if (!inst) return nullptr;
        inst->_current = nullptr;
        inst->_coroutine = nullptr;
        std::swap(this->_coroutine, inst->_coroutine);
        return inst;
    } catch (...) {
        BNM_LOG_ERR("IEnumerator::Get exception: %s", Internal::GetExceptionTypeName());
        return nullptr;
    }
}

void BNM::Coroutine::IEnumerator::Reset() {}

BNM::IL2CPP::Il2CppObject *BNM::Coroutine::IEnumerator::Current() {
    return _current;
}

BNM::Coroutine::AsyncOperation::AsyncOperation(intptr_t operation) {
    _object = Internal::coroutineAsyncOperation.CreateNewObjectParameters(operation);
}

BNM::Coroutine::WaitForEndOfFrame::WaitForEndOfFrame() {
    _object = Internal::coroutineWaitForEndOfFrame.CreateNewInstance();
}

BNM::Coroutine::WaitForFixedUpdate::WaitForFixedUpdate() {
    _object = Internal::coroutineWaitForFixedUpdate.CreateNewInstance();
}

BNM::Coroutine::WaitForSeconds::WaitForSeconds(float seconds) {
    _object = Internal::coroutineWaitForSeconds.CreateNewObjectParameters(seconds);
}

BNM::Coroutine::WaitForSecondsRealtime::WaitForSecondsRealtime(float seconds) {
    _object = Internal::coroutineWaitForSecondsRealtime.CreateNewObjectParameters(seconds);
}

BNM::Coroutine::WaitUntil::WaitUntil(const std::function<bool()> &function) {
    auto obj = (CustomWait *) BNM::Class(Internal::coroutineWaitClass.myClass).CreateNewInstance();
    if (obj) {
        obj->_func = function;
        obj->_isUntil = true;
    }
    _object = obj;
}

BNM::Coroutine::WaitWhile::WaitWhile(const std::function<bool()> &function) {
    auto obj = (CustomWait *) BNM::Class(Internal::coroutineWaitClass.myClass).CreateNewInstance();
    if (obj) {
        obj->_func = function;
        obj->_isUntil = false;
    }
    _object = obj;
}

void Utils::LogCompileTimeClass(const CompileTimeClass &compileTimeClass) {
    for (auto info : compileTimeClass._stack) {
        switch (info->_baseType) {
            case CompileTimeClass::_BaseType::Class: {
                auto classInfo = (CompileTimeClass::_ClassInfo *) info; (void) classInfo;
                BNM_LOG_ERR("\tClass( imageName: \"%s\", namespace: \"%s\", name: \"%s\") - %s", classInfo->_imageName, classInfo->_namespace, classInfo->_name, compileTimeClass._loadedClass.str().data());
            } break;
            case CompileTimeClass::_BaseType::Inner: {
                auto innerInfo = (CompileTimeClass::_InnerInfo *) info; (void) innerInfo;
                BNM_LOG_ERR("\tInner( name: \"%s\") - %s", innerInfo->_name, compileTimeClass._loadedClass.str().data());
            } break;
            case CompileTimeClass::_BaseType::Modifier:
            case CompileTimeClass::_BaseType::Generic:
            case CompileTimeClass::_BaseType::None:
            case CompileTimeClass::_BaseType::MaxCount: break;
        }
    }
}

void Loading::AddOnLoadedEvent(void (*event)()) {
    if (event) Internal::onLoadedEvents.push_back(event);
}

void Loading::ClearOnLoadedEvents() {
    Internal::onLoadedEvents.clear();
}
