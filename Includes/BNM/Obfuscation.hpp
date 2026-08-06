#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace BNM::Obfuscation {

constexpr uint32_t Fnv1a32(const char *str, size_t n) {
    uint32_t h = 2166136261u;
    for (size_t i = 0; i < n; ++i) h = (h ^ (uint8_t) str[i]) * 16777619u;
    return h;
}

constexpr uint64_t Mix64(uint64_t x) {
    x ^= x >> 30;
    x *= 0xbf58476d1ce4e5b9ull;
    x ^= x >> 27;
    x *= 0x94d049bb133111ebull;
    x ^= x >> 31;
    return x;
}

template<size_t N>
struct Block {
    uint64_t keyA;
    uint64_t keyB;
    uint8_t rot;
    uint8_t blob[N];

    constexpr Block(const char (&str)[N], uint64_t a, uint64_t b) : keyA(a), keyB(b), rot((uint8_t) ((a >> 42) % (uint64_t) N)) {
        for (size_t i = 0; i < N; ++i) {
            uint64_t k = Mix64(a ^ (uint64_t) i * b + b);
            uint8_t c = (uint8_t) str[i];
            blob[(i + rot) % N] = (uint8_t) (c ^ (uint8_t) (k >> 56) ^ (uint8_t) (k >> 32) ^ (uint8_t) (k >> 8) ^ (uint8_t) k ^ (uint8_t) (b >> ((i % 8) * 8)));
        }
    }

    __attribute__((noinline)) size_t Restore(char *out) const {
        for (size_t i = 0; i < N; ++i) {
            uint64_t k = Mix64(keyA ^ (uint64_t) i * keyB + keyB);
            uint8_t c = ((volatile uint8_t *) blob)[(i + rot) % N];
            ((volatile uint8_t *) out)[i] = (uint8_t) (c ^ (uint8_t) (k >> 56) ^ (uint8_t) (k >> 32) ^ (uint8_t) (k >> 8) ^ (uint8_t) k ^ (uint8_t) (keyB >> ((i % 8) * 8)));
        }
        return N;
    }
};

constexpr uint32_t BuildSalt() { return Fnv1a32(__TIME__, 8); }



template<size_t N>
constexpr size_t StrSize(const char (&)[N]) { return N; }

}

#define BNM_OBFUSCATE(str) \
    ([]() { \
        constexpr auto bnm_obf_block = ::BNM::Obfuscation::Block<::BNM::Obfuscation::StrSize(str)>(str, \
            ::BNM::Obfuscation::Mix64((uint64_t) ::BNM::Obfuscation::Fnv1a32(str, sizeof(str) - 1) ^ (uint64_t) ::BNM::Obfuscation::BuildSalt() ^ (uint64_t) (__COUNTER__) * 0x9e3779b97f4a7c15ull), \
            ::BNM::Obfuscation::Mix64((uint64_t) (__LINE__) * 0x9e3779b97f4a7c15ull ^ (uint64_t) (__COUNTER__) ^ (uint64_t) ::BNM::Obfuscation::BuildSalt())); \
        static thread_local char bnm_obf_buf[::BNM::Obfuscation::StrSize(str)]; \
        using bnm_obf_fn_t = size_t (::BNM::Obfuscation::Block<::BNM::Obfuscation::StrSize(str)>::*)(char *) const; \
        static bnm_obf_fn_t bnm_obf_restore = &::BNM::Obfuscation::Block<::BNM::Obfuscation::StrSize(str)>::Restore; \
        (bnm_obf_block.*bnm_obf_restore)(bnm_obf_buf); \
        return bnm_obf_buf; \
    }())

#define BNM_OBFUSCATE_TMP(str) BNM_OBFUSCATE(str)