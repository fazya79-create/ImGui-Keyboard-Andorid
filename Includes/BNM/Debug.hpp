#pragma once

#pragma once
#if __cplusplus < 202002L
#error "BNM-Free requires C++20 and above!"
#endif

#include <cstdint>
#include <cmath>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <csetjmp>
#include <csignal>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <utility>
#include <functional>
#include <type_traits>
#include <initializer_list>
#include <limits>
#include <coroutine>
#include <dlfcn.h>
#include <android/log.h>
#include <jni.h>

#if defined(__LP64__)
typedef long BNM_INT_PTR;
typedef unsigned long BNM_PTR;
#else
typedef int BNM_INT_PTR;
typedef unsigned int BNM_PTR;
#endif

#include "Obfuscation.hpp"
#define BNM_VER "1.0.0"

#define BNM_LOG_INFO(...) ((void)__android_log_print(4, "BNM-Free", __VA_ARGS__))
#define BNM_LOG_DEBUG(...) ((void)__android_log_print(3, "BNM-Free", __VA_ARGS__))
#define BNM_LOG_DEBUG_IF(condition, ...) if (condition) ((void)__android_log_print(3, "BNM-Free", __VA_ARGS__))
#define BNM_LOG_ERR(...) ((void)__android_log_print(6, "BNM-Free", __VA_ARGS__))
#define BNM_LOG_ERR_IF(condition, ...) if (condition) ((void)__android_log_print(6, "BNM-Free", __VA_ARGS__))
#define BNM_LOG_WARN(...) ((void)__android_log_print(5, "BNM-Free", __VA_ARGS__))
#define BNM_LOG_WARN_IF(condition, ...) if (condition) ((void)__android_log_print(5, "BNM-Free", __VA_ARGS__))

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
