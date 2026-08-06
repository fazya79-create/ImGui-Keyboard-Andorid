#pragma once

#include "Debug.hpp"
#include "Il2CppStructures.hpp"
#include "Internals.hpp"
namespace BNM {
namespace Structures {

namespace Mono {

struct String {
    IL2CPP::Il2CppObject object;
    int32_t length;
    IL2CPP::Il2CppChar chars[32];

    inline std::string str() const;
    inline unsigned int GetHash() const;
    static String *Empty();
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-bool-conversion"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnonnull-compare"
#endif
    inline bool IsNullOrEmpty() const { return !this || !length; }
#if defined(__clang__)
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
    inline operator std::string() const { return str(); }
};

template<typename T>
struct Array {
    IL2CPP::Il2CppObject obj;
    IL2CPP::Il2CppArrayBounds *bounds;
    IL2CPP::il2cpp_array_size_t max_length;

    inline T *At(IL2CPP::il2cpp_array_size_t index) { return (T *) ((char *) this + sizeof(IL2CPP::Il2CppArray) + sizeof(T) * index); }
    inline T &operator[](IL2CPP::il2cpp_array_size_t index) { return *(T *) ((char *) this + sizeof(IL2CPP::Il2CppArray) + sizeof(T) * index); }
    inline IL2CPP::il2cpp_array_size_t Size() const { return max_length; }
    inline IL2CPP::il2cpp_array_size_t GetCapacity() const { return max_length; }
    inline T *GetData() { return GetItems(); }
    inline std::vector<T> ToVector() const {
        std::vector<T> ret;
        for (IL2CPP::il2cpp_array_size_t i = 0; i < max_length; ++i) ret.push_back((*const_cast<Array<T> *>(this))[i]);
        return ret;
    }
    inline void Destroy() { Internal::api.il2cpp_free(this); }
    inline T *GetItems() { return (T *) ((char *) this + sizeof(IL2CPP::Il2CppArray)); }
    static Array<T> *Create(IL2CPP::il2cpp_array_size_t size, bool zeroed = false);
    template<typename ...Args>
    static Array<T> *CreateWithItems(Args &&...args) {
        auto arr = Create(sizeof...(Args), false);
        T values[] = {static_cast<T>(args)...};
        for (IL2CPP::il2cpp_array_size_t i = 0; i < sizeof...(Args); ++i) (*arr)[i] = values[i];
        return arr;
    }
    inline void CopyFrom(T *source, IL2CPP::il2cpp_array_size_t count) {
        for (IL2CPP::il2cpp_array_size_t i = 0; i < count; ++i) (*this)[i] = source[i];
    }
};

namespace PRIVATE_MonoListData {
    void *CompareExchange4List(void *syncRoot);
}

template<typename T>
struct DataIterator {
    T *value{};
    inline constexpr DataIterator() = default;
    inline constexpr DataIterator(const T *value) : value((T *) value) {}
    inline T &operator*() const { return *value; }
    inline T *operator->() const { return value; }
    inline operator T &() const { return *value; }
    inline DataIterator &operator++() { ++value; return *this; }
    inline DataIterator operator++(int) { auto tmp = *this; ++value; return tmp; }
    inline bool operator==(const DataIterator &other) const { return value == other.value; }
    inline bool operator!=(const DataIterator &other) const { return value != other.value; }
};

template<typename T>
struct List {
    IL2CPP::Il2CppObject obj;
    Mono::Array<T> *_items;
    int32_t _size;
    int32_t _version;
    IL2CPP::Il2CppObject *_syncRoot;

    inline constexpr List() : obj(), _items(nullptr), _size(0), _version(0), _syncRoot(nullptr) {}

    inline T &operator[](int32_t index) { return (*_items)[index]; }
    inline T *GetItems() { return _items ? _items->GetItems() : nullptr; }
    inline int32_t Count() const { return _size; }
    inline int32_t GetSize() const { return _size; }
    inline int32_t GetVersion() const { return _version; }
    inline bool GetFalse() const { return false; }
    inline std::vector<T> ToVector() const {
        std::vector<T> ret;
        for (int32_t i = 0; i < _size; ++i) ret.push_back((*const_cast<List<T> *>(this))[i]);
        return ret;
    }

    inline void Resize(int32_t newSize) {
        if (newSize < 1) newSize = 1;
        if (!_items) {
            _items = Mono::Array<T>::Create(newSize);
            return;
        }
        auto newArr = Mono::Array<T>::Create(newSize);
        auto copyCount = _size < newSize ? _size : newSize;
        for (int32_t i = 0; i < copyCount; ++i) (*newArr)[i] = (*_items)[i];
        _items = newArr;
    }
    inline void GrowIfNeeded(int32_t n) { if (_size + n > (int32_t) _items->Size()) Resize(_size + n); }

    inline void Add(T item) {
        if (!_items) {
            _items = Mono::Array<T>::Create(4);
            _size = 0;
        }
        if (_size >= (int32_t) _items->Size()) Resize(_size * 2 + 1);
        (*_items)[_size++] = item;
        ++_version;
    }

    inline void Shift(int32_t start, int32_t delta) {
        auto oldSize = _size;
        if (delta < 0) start -= delta;
        if (start < _size) memmove(_items->GetItems() + start + delta, _items->GetItems() + start, (_size - start) * sizeof(T));
        _size += delta;
        if (delta < 0) memset(_items->GetItems() + oldSize + delta, 0, -delta * sizeof(T));
    }

    inline void RemoveAt(int32_t index) {
        if (index < 0 || index >= _size) return;
        Shift(index, -1);
        ++_version;
    }

    inline bool Remove(T item) {
        auto idx = IndexOf(item);
        if (idx == -1) return false;
        RemoveAt(idx);
        return true;
    }

    inline void Clear() {
        if (_items) memset(_items->GetItems(), 0, _size * sizeof(T));
        _size = 0;
        ++_version;
    }

    inline T GetItem(int32_t index) const {
        if (index >= _size) return {};
        return (*_items)[index];
    }
    inline void SetItem(int32_t index, T item) {
        if (index >= _size) return;
        (*_items)[index] = item;
        ++_version;
    }

    inline void Insert(int32_t index, T item) {
        if (index > _size) return;
        if (_size == (int32_t) _items->Size()) GrowIfNeeded(1);
        if (index < _size) memmove(_items->GetItems() + index + 1, _items->GetItems() + index, (_size - index) * sizeof(T));
        (*_items)[index] = item;
        ++_size;
        ++_version;
    }

    inline int32_t IndexOf(T item) const {
        for (int32_t i = 0; i < _size; ++i) if ((*_items)[i] == item) return i;
        return -1;
    }

    inline bool Contains(T item) const { return IndexOf(item) != -1; }

    inline void CopyTo(Array<T> *arr, int32_t arrIndex) const {
        if (!_items || !arr) return;
        memcpy(_items->GetItems(), arr->GetItems() + arrIndex, _size * sizeof(T));
    }

    inline void *GetSyncRoot() { if (!_syncRoot) _syncRoot = (IL2CPP::Il2CppObject *) PRIVATE_MonoListData::CompareExchange4List(_syncRoot); return _syncRoot; }
    inline void *GetEnumerator() { return this; }
    inline DataIterator<T> begin() { return DataIterator<T>(GetItems()); }
    inline DataIterator<T> end() { return DataIterator<T>(_items ? GetItems() + _size : nullptr); }
};

namespace PRIVATE_MonoListData {
    struct MethodData { const char *name{}; void *ptr{}; };

    template<typename T> constexpr std::string_view WrappedTypeName() { return __PRETTY_FUNCTION__; }
    constexpr std::size_t WrappedTypeNamePrefixLength() { return WrappedTypeName<void>().find("void"); }
    constexpr std::size_t WrappedTypeNameSuffixLength() { return WrappedTypeName<void>().length() - WrappedTypeNamePrefixLength() - 4; }
    constexpr uint32_t FNV1a(const char *str, size_t n, uint32_t hash = 2166136261U) {
        return n == 0 ? hash : FNV1a(str + 1, n - 1, (hash ^ str[0]) * 19777619U);
    }
    constexpr uint32_t FNV1a(const std::string_view &str) { return FNV1a(str.data(), str.size()); }
    template<typename T> constexpr uint32_t HashedTypeName() {
        constexpr auto wrapped = WrappedTypeName<T>();
        constexpr auto prefix = WrappedTypeNamePrefixLength();
        constexpr auto suffix = WrappedTypeNameSuffixLength();
        constexpr auto name = wrapped.substr(prefix, wrapped.length() - prefix - suffix);
        return FNV1a(name);
    }

    IL2CPP::Il2CppClass *TryGetMonoListClass(uint32_t typeHash, MethodData *data, size_t count);
    void *CompareExchange4List(void *syncRoot);

    template<typename T>
    void InitMonoListVTable(List<T> *list) {
        using Type = std::conditional_t<std::is_pointer_v<T>, void *, T>;
        constexpr auto RemoveAt = &List<Type>::RemoveAt;
        constexpr auto GetSize = &List<Type>::GetSize;
        constexpr auto Clear = &List<Type>::Clear;
        constexpr auto get_Item = &List<Type>::GetItem;
        constexpr auto set_Item = &List<Type>::SetItem;
        constexpr auto IndexOf = &List<Type>::IndexOf;
        constexpr auto Insert = &List<Type>::Insert;
        constexpr auto get_false = &List<Type>::GetFalse;
        constexpr auto Add = &List<Type>::Add;
        constexpr auto Contains = &List<Type>::Contains;
        constexpr auto CopyTo = &List<Type>::CopyTo;
        constexpr auto Remove = &List<Type>::Remove;
        constexpr auto GetEnumerator = &List<Type>::GetEnumerator;
        constexpr auto GetSyncRoot = &List<Type>::GetSyncRoot;
        static std::array<MethodData, 16> namesMap = {
            MethodData{"RemoveAt", *(void **) &RemoveAt}, MethodData{"get_Count", *(void **) &GetSize},
            MethodData{"Clear", *(void **) &Clear}, MethodData{"get_Item", *(void **) &get_Item},
            MethodData{"set_Item", *(void **) &set_Item}, MethodData{"IndexOf", *(void **) &IndexOf},
            MethodData{"Insert", *(void **) &Insert}, MethodData{"get_IsReadOnly", *(void **) &get_false},
            MethodData{"get_IsFixedSize", *(void **) &get_false}, MethodData{"get_IsSynchronized", *(void **) &get_false},
            MethodData{"Add", *(void **) &Add}, MethodData{"Contains", *(void **) &Contains},
            MethodData{"CopyTo", *(void **) &CopyTo}, MethodData{"Remove", *(void **) &Remove},
            MethodData{"GetEnumerator", *(void **) &GetEnumerator}, MethodData{"get_SyncRoot", *(void **) &GetSyncRoot}
        };
        list->obj.klass = TryGetMonoListClass(HashedTypeName<Type>(), namesMap.data(), namesMap.size());
    }
}

}
}
}
