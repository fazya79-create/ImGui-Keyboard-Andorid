#include "BNM.hpp"

using namespace BNM;

namespace BNM::Internal::AssemblerUtils {

static std::string ReverseHexString(const std::string &hex) {
    std::string out{};
    for (size_t i = 0; i < hex.length(); i += 2) out.insert(0, hex.substr(i, 2));
    return out;
}

static BNM_PTR HexStr2Value(const std::string &hex) {
    return strtoull(hex.c_str(), nullptr, 16);
}

#if defined(__ARM_ARCH_7A__)

static bool IsBranchHex(const std::string &hex) {
    BNM_PTR hexW = HexStr2Value(ReverseHexString(hex));
    return (hexW & 0x0A000000) == 0x0A000000;
}

#elif defined(__aarch64__)

static bool IsBranchHex(const std::string &hex) {
    BNM_PTR hexW = HexStr2Value(ReverseHexString(hex));
    return (hexW & 0xFC000000) == 0x14000000 || (hexW & 0xFC000000) == 0x94000000;
}

#elif defined(__i386__) || defined(__x86_64__)

static bool IsBranchHex(const std::string &hex) {
    return hex.size() >= 2 && hex[0] == 'E' && hex[1] == '8';
}

#else
#error "BNM-Free only supports arm64, arm, x86 and x86_64"
#endif

static const char *hexChars = "0123456789ABCDEF";

template<size_t len>
static std::string ReadMemory(BNM_PTR address) {
    char temp[len];
    memset(temp, 0, len);
    std::string ret{};
    if (memcpy(temp, (void *) address, len) == nullptr) return ret;
    ret.resize(len * 2, 0);
    auto buf = (char *) ret.data();
    for (size_t i = 0; i < len; ++i) {
        *buf++ = hexChars[((uint8_t) temp[i]) >> 4];
        *buf++ = hexChars[((uint8_t) temp[i]) & 0x0F];
    }
    return ret;
}

static bool DecodeBranchOrCall(const std::string &hex, BNM_PTR offset, BNM_PTR &outOffset) {
#if defined(__ARM_ARCH_7A__) || defined(__aarch64__)
    if (!IsBranchHex(hex)) return false;
#if defined(__aarch64__)
    uint8_t add = 0;
#else
    uint8_t add = 8;
#endif
    outOffset = ((int32_t) (((((HexStr2Value(ReverseHexString(hex))) & (((uint32_t) 1 << 24) - 1) << 0) >> 0) << 2) << (32 - 26)) >> (32 - 26)) + offset + add;
#elif defined(__i386__) || defined(__x86_64__)
    if (!IsBranchHex(hex)) return false;
    outOffset = offset + HexStr2Value(ReverseHexString(hex).substr(0, 8)) + 5;
#else
#error "BNM-Free only supports arm64, arm, x86 and x86_64"
    return false;
#endif
    return true;
}

BNM_PTR FindNextJump(BNM_PTR start, uint8_t index) {
#if defined(__ARM_ARCH_7A__) || defined(__aarch64__)
    BNM_PTR offset = 0;
    std::string curHex = ReadMemory<4>(start);
    BNM_PTR outOffset = 0;
    bool out = false;
    while (!(out = DecodeBranchOrCall(curHex, start + offset, outOffset)) || index != 1) {
        offset += 4;
        curHex = ReadMemory<4>(start + offset);
        if (out) index--;
    }
    return outOffset;
#elif defined(__i386__) || defined(__x86_64__)
    BNM_PTR offset = 0;
    std::string curHex = ReadMemory<1>(start);
    BNM_PTR outOffset = 0;
    bool out = false;
    while (!(out = IsBranchHex(curHex)) || index != 1) {
        offset += 1;
        curHex = ReadMemory<1>(start + offset);
        if (out) index--;
    }
    DecodeBranchOrCall(ReadMemory<5>(start + offset), start + offset, outOffset);
    return outOffset;
#else
#error "BNM-Free only supports arm64, arm, x86 and x86_64"
    return 0;
#endif
}

}

using namespace BNM::Internal::AssemblerUtils;

void *BNM::Internal::AssemblerUtils::FindJump(void *start, uint8_t count) {
    if (!start) return nullptr;
    return (void *) FindNextJump((BNM_PTR) start, count);
}

void BNM::EmptyMethod() {}

void *BNM::OffsetInLib(void *offsetInMemory) {
    if (offsetInMemory == nullptr) return nullptr;
    Dl_info info;
    BNM_dladdr(offsetInMemory, &info);
    return (void *) ((BNM_PTR) offsetInMemory - (BNM_PTR) info.dli_fbase);
}

void *Utils::OffsetInLib(void *offsetInMemory) {
    return ::OffsetInLib(offsetInMemory);
}

bool BNM::CheckHandle(void *handle) {
    void *init = BNM_dlsym(handle, BNM_OBFUSCATE_TMP("il2cpp_init"));
    if (!init) return false;

    Internal::BNM_il2cpp_init_origin = ::BasicHook(init, (void *) Internal::BNM_il2cpp_init, Internal::old_BNM_il2cpp_init);

    if (Internal::states.lateInitAllowed) Internal::LateInit(BNM_dlsym(handle, BNM_OBFUSCATE_TMP("il2cpp_class_from_il2cpp_type")));

    Internal::il2cppLibraryHandle = handle;
    return true;
}

#if defined(__ARM_ARCH_7A__)
#define CURRENT_ARCH "armeabi-v7a"
#elif defined(__aarch64__)
#define CURRENT_ARCH "arm64-v8a"
#elif defined(__i386__)
#define CURRENT_ARCH "x86"
#elif defined(__x86_64__)
#define CURRENT_ARCH "x86_64"
#elif defined(__riscv)
#define CURRENT_ARCH "riscv64"
#endif
