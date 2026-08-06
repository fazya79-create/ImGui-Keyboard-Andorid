#pragma once

#include "Class.hpp"
namespace BNM {
struct PropertyBase {
    IL2CPP::PropertyInfo *_data{};
    IL2CPP::Il2CppObject *_instance{};
    uint8_t _hasGetter : 1 = false;
    uint8_t _hasSetter : 1 = false;
    MethodBase _getter{};
    MethodBase _setter{};

    inline constexpr PropertyBase() = default;
    inline PropertyBase(const PropertyBase &other) = default;
    PropertyBase(const IL2CPP::PropertyInfo *info);
    inline IL2CPP::PropertyInfo *GetInfo() const { return _data; }
    inline bool IsValid() const { return _data != nullptr; }
    inline operator bool() const { return IsValid(); }
    PropertyBase &SetInstance(IL2CPP::Il2CppObject *val);
    BNM::Class GetType() const;
    BNM::Class GetParentClass() const;
    inline std::string str() const {
        if (!_data) return "nullptr";
        return _data->name ? _data->name : "nullptr";
    }
    inline PropertyBase &operator[](void *instance) { SetInstance((IL2CPP::Il2CppObject *) instance); return *this; }
    inline PropertyBase &operator[](IL2CPP::Il2CppObject *instance) { SetInstance(instance); return *this; }
    template<typename NewType>
    inline NewType &cast() const { return (NewType &) *this; }
};

template<typename T = bool>
struct Property : public PropertyBase {
    inline constexpr Property() = default;
    template<typename OtherType>
    inline Property(const Property<OtherType> &other) : PropertyBase(other) {}
    inline Property(const IL2CPP::PropertyInfo *info) : PropertyBase(info) {}
    inline Property(const PropertyBase &other) : PropertyBase(other) {}

    inline Property<T> &operator[](void *instance) { SetInstance((IL2CPP::Il2CppObject *) instance); return *this; }
    inline Property<T> &operator[](IL2CPP::Il2CppObject *instance) { SetInstance(instance); return *this; }

    inline T Get() const {
        if (_hasGetter) return _getter.cast<Method<T>>()();
        BNM_LOG_ERR("Property::Get no getter: %s", str().c_str());
        return PRIVATE_INTERNAL::ReturnEmpty<T>();
    }
    inline operator T() const { return Get(); }
    inline T operator()() const { return Get(); }

    inline void Set(T value) {
        if (_hasSetter) {
            _setter.cast<Method<void>>()(value);
            return;
        }
        BNM_LOG_ERR("Property::Set no setter: %s", str().c_str());
    }
    inline Property<T> &operator=(T value) { Set(std::move(value)); return *this; }
    inline Property<T> &operator=(const PropertyBase &other) {
        _data = other._data;
        _instance = other._instance;
        _hasGetter = other._hasGetter;
        _hasSetter = other._hasSetter;
        _getter = other._getter;
        _setter = other._setter;
        return *this;
    }
};

struct EventBase {
    IL2CPP::EventInfo *_data{};
    IL2CPP::Il2CppObject *_instance{};
    uint8_t _hasAdd : 1 = false;
    uint8_t _hasRemove : 1 = false;
    uint8_t _hasRaise : 1 = false;
    MethodBase _add{};
    MethodBase _remove{};
    MethodBase _raise{};

    inline constexpr EventBase() = default;
    inline EventBase(const EventBase &other) = default;
    EventBase(const IL2CPP::EventInfo *info);
    inline IL2CPP::EventInfo *GetInfo() const { return _data; }
    inline bool IsValid() const { return _data != nullptr; }
    inline operator bool() const { return IsValid(); }
    EventBase &SetInstance(IL2CPP::Il2CppObject *instance);
    BNM::Class GetParentClass() const;
    inline std::string str() const {
        if (!_data) return "nullptr";
        return _data->name ? _data->name : "nullptr";
    }
    inline EventBase &operator[](void *instance) { SetInstance((IL2CPP::Il2CppObject *) instance); return *this; }
    inline EventBase &operator[](IL2CPP::Il2CppObject *instance) { SetInstance(instance); return *this; }
    template<typename NewType>
    inline NewType &cast() const { return (NewType &) *this; }
};

template<typename Ret = void, typename ...Parameters>
struct Event : public EventBase {
    inline constexpr Event() = default;
    template<typename OtherType>
    inline Event(const Event<OtherType> &other) : EventBase(other) {}
    inline Event(const IL2CPP::EventInfo *info) : EventBase(info) {}
    inline Event(const EventBase &other) : EventBase(other) {}

    inline Event<Ret, Parameters...> &operator[](void *instance) { SetInstance((IL2CPP::Il2CppObject *) instance); return *this; }
    inline Event<Ret, Parameters...> &operator[](IL2CPP::Il2CppObject *instance) { SetInstance(instance); return *this; }

    inline void Add(DelegateBase *delegate) {
        if (_hasAdd) {
            _add.cast<Method<void>>()(delegate);
            return;
        }
        BNM_LOG_ERR("Event::Add no add method: %s", str().c_str());
    }
    inline Event<Ret, Parameters...> &operator+=(DelegateBase *delegate) { Add(delegate); return *this; }

    inline void Remove(DelegateBase *delegate) {
        if (_hasRemove) {
            _remove.cast<Method<void>>()(delegate);
            return;
        }
        BNM_LOG_ERR("Event::Remove no remove method: %s", str().c_str());
    }
    inline Event<Ret, Parameters...> &operator-=(DelegateBase *delegate) { Remove(delegate); return *this; }

    inline Ret Raise(Parameters ...parameters) const {
        if (_hasRaise) return _raise.cast<Method<Ret>>()(parameters...);
        BNM_LOG_ERR("Event::Raise no raise method: %s", str().c_str());
        return PRIVATE_INTERNAL::ReturnEmpty<Ret>();
    }
    inline Ret operator()(Parameters ...parameters) const { return Raise(parameters...); }
    inline Event<Ret> &operator=(const EventBase &other) {
        _data = other._data;
        _instance = other._instance;
        _hasAdd = other._hasAdd;
        _hasRemove = other._hasRemove;
        _hasRaise = other._hasRaise;
        _add = other._add;
        _remove = other._remove;
        _raise = other._raise;
        return *this;
    }
};
}
