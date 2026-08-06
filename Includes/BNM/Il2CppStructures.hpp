#pragma once

#include "Debug.hpp"
namespace BNM {

namespace IL2CPP {

typedef void (*Il2CppMethodPointer)();
typedef size_t il2cpp_array_size_t;
typedef int32_t il2cpp_array_lower_bound_t;
typedef uint16_t Il2CppChar;
typedef uint32_t Il2CppGCHandle;
typedef int32_t TypeDefinitionIndex;
typedef int32_t GenericParameterIndex;
typedef int32_t ImageIndex;
typedef int32_t il2cpp_hresult_t;
typedef int32_t Il2CppStat;
typedef int32_t Il2CppGCMode;
typedef int32_t Il2CppRuntimeUnhandledExceptionPolicy;
typedef int32_t Il2CppProfileFlags;
typedef uint32_t Il2CppProfileFunc;
typedef uint32_t Il2CppProfileMethodFunc;
typedef uint32_t Il2CppProfileAllocFunc;
typedef uint32_t Il2CppProfileGCFunc;
typedef uint32_t Il2CppProfileGCResizeFunc;
typedef uint32_t Il2CppProfileFileIOFunc;
typedef uint32_t Il2CppProfileThreadFunc;
typedef uint32_t Il2CppFrameWalkFunc;
typedef uint32_t Il2CppBacktraceFunc;
typedef uint32_t il2cpp_register_object_callback;
typedef uint32_t il2cpp_liveness_reallocate_callback;
typedef uint32_t il2cpp_WorldChangedCallback;
typedef uint32_t Il2CppSetFindPlugInCallback;
typedef uint32_t Il2CppLogCallback;

struct Il2CppMemoryCallbacks;
struct Il2CppProfiler;
struct Il2CppStackFrameInfo;
struct Il2CppMethodDebugInfo;
struct Il2CppCustomAttrInfo;
struct Il2CppManagedMemorySnapshot;
struct Il2CppDebuggerTransport;
struct Il2CppInternalThread;
struct Il2CppAppDomain;
struct Il2CppAppDomainSetup;
struct Il2CppAppContext;
struct Il2CppDelegate;
struct Il2CppVTable;
struct MonitorData;
struct Il2CppNameToTypeHandleHashTable;
struct Il2CppCodeGenModule;
struct Il2CppInteropData;
struct Il2CppRGCTXData;
struct ___Il2CppMetadataTypeHandle;
struct ___Il2CppMetadataGenericParameterHandle;
struct Il2CppMetadataRegistration;
struct Il2CppCodeRegistration;
struct Il2CppGenericClass;
struct Il2CppGenericInst;
struct Il2CppGenericMethod;
struct Il2CppType;
struct Il2CppClass;
struct MethodInfo;
struct FieldInfo;
struct PropertyInfo;
struct EventInfo;
struct Il2CppImage;
struct Il2CppAssembly;
struct Il2CppArray;
struct Il2CppString;
struct Il2CppObject;
struct Il2CppException;
struct Il2CppReflectionType;
struct Il2CppReflectionMethod;
struct Il2CppThread;
struct Il2CppDomain;
struct Il2CppArrayBounds;

typedef const ___Il2CppMetadataTypeHandle *Il2CppMetadataTypeHandle;
typedef const ___Il2CppMetadataGenericParameterHandle *Il2CppMetadataGenericParameterHandle;
typedef const void *Il2CppMetadataImageHandle;
typedef const void *Il2CppMetadataGenericContainerHandle;
typedef const void *Il2CppMetadataMethodDefinitionHandle;

enum Il2CppTypeEnum : int {
    IL2CPP_TYPE_END = 0x00,
    IL2CPP_TYPE_VOID = 0x01,
    IL2CPP_TYPE_BOOLEAN = 0x02,
    IL2CPP_TYPE_CHAR = 0x03,
    IL2CPP_TYPE_I1 = 0x04,
    IL2CPP_TYPE_U1 = 0x05,
    IL2CPP_TYPE_I2 = 0x06,
    IL2CPP_TYPE_U2 = 0x07,
    IL2CPP_TYPE_I4 = 0x08,
    IL2CPP_TYPE_U4 = 0x09,
    IL2CPP_TYPE_I8 = 0x0A,
    IL2CPP_TYPE_U8 = 0x0B,
    IL2CPP_TYPE_R4 = 0x0C,
    IL2CPP_TYPE_R8 = 0x0D,
    IL2CPP_TYPE_STRING = 0x0E,
    IL2CPP_TYPE_PTR = 0x0F,
    IL2CPP_TYPE_BYREF = 0x10,
    IL2CPP_TYPE_VALUETYPE = 0x11,
    IL2CPP_TYPE_CLASS = 0x12,
    IL2CPP_TYPE_VAR = 0x13,
    IL2CPP_TYPE_ARRAY = 0x14,
    IL2CPP_TYPE_GENERICINST = 0x15,
    IL2CPP_TYPE_TYPEDBYREF = 0x16,
    IL2CPP_TYPE_I = 0x18,
    IL2CPP_TYPE_U = 0x19,
    IL2CPP_TYPE_FNPTR = 0x1B,
    IL2CPP_TYPE_OBJECT = 0x1C,
    IL2CPP_TYPE_SZARRAY = 0x1D,
    IL2CPP_TYPE_MVAR = 0x1E,
    IL2CPP_TYPE_CMOD_REQD = 0x1F,
    IL2CPP_TYPE_CMOD_OPT = 0x20,
    IL2CPP_TYPE_INTERNAL = 0x21,
    IL2CPP_TYPE_MODIFIER = 0x40,
    IL2CPP_TYPE_SENTINEL = 0x41,
    IL2CPP_TYPE_PINNED = 0x45,
    IL2CPP_TYPE_ENUM = 0x55,
    IL2CPP_TYPE_IL2CPP_TYPE_INDEX = 0xFF
};

typedef struct Il2CppArrayType {
    const Il2CppType *etype;
    uint8_t rank;
    uint8_t numsizes;
    uint8_t numlobounds;
    int *sizes;
    int *lobounds;
} Il2CppArrayType;

typedef struct Il2CppGenericInst {
    uint32_t type_argc;
    const Il2CppType **type_argv;
} Il2CppGenericInst;

typedef struct Il2CppGenericContext {
    const Il2CppGenericInst *class_inst;
    const Il2CppGenericInst *method_inst;
} Il2CppGenericContext;

typedef struct Il2CppGenericClass {
    const Il2CppType *type;
    Il2CppGenericContext context;
    Il2CppClass *cached_class;
} Il2CppGenericClass;

typedef struct Il2CppGenericMethod {
    const MethodInfo *methodDefinition;
    Il2CppGenericContext context;
} Il2CppGenericMethod;

typedef struct Il2CppType {
    union {
        void *dummy;
        TypeDefinitionIndex __klassIndex;
        Il2CppMetadataTypeHandle typeHandle;
        const Il2CppType *type;
        Il2CppArrayType *array;
        GenericParameterIndex __genericParameterIndex;
        Il2CppMetadataGenericParameterHandle genericParameterHandle;
        Il2CppGenericClass *generic_class;
    } data;
    unsigned int attrs : 16;
    Il2CppTypeEnum type : 8;
    unsigned int num_mods : 5;
    unsigned int byref : 1;
    unsigned int pinned : 1;
    unsigned int valuetype : 1;
} Il2CppType;

typedef struct Il2CppObject {
    union {
        Il2CppClass *klass;
        Il2CppVTable *vtable;
    };
    MonitorData *monitor;
} Il2CppObject;

typedef struct Il2CppArrayBounds {
    IL2CPP::il2cpp_array_size_t length;
    il2cpp_array_lower_bound_t lower_bound;
} Il2CppArrayBounds;

typedef struct Il2CppArray {
    Il2CppObject obj;
    Il2CppArrayBounds *bounds;
    IL2CPP::il2cpp_array_size_t max_length;
} Il2CppArray;

typedef struct Il2CppString {
    Il2CppObject object;
    int32_t length;
    Il2CppChar chars[32];
} Il2CppString;

typedef struct Il2CppReflectionType {
    Il2CppObject object;
    const Il2CppType *type;
} Il2CppReflectionType;

typedef struct Il2CppReflectionMethod {
    Il2CppObject object;
    const MethodInfo *method;
    Il2CppString *name;
    Il2CppReflectionType *reftype;
} Il2CppReflectionMethod;

typedef struct Il2CppRuntimeInterfaceOffsetPair {
    Il2CppClass *interfaceType;
    int32_t offset;
} Il2CppRuntimeInterfaceOffsetPair;

typedef struct VirtualInvokeData {
    Il2CppMethodPointer methodPtr;
    const MethodInfo *method;
} VirtualInvokeData;

typedef struct Il2CppClass {
    const Il2CppImage *image;
    void *gc_desc;
    const char *name;
    const char *namespaze;
    Il2CppType byval_arg;
    Il2CppType this_arg;
    Il2CppClass *element_class;
    Il2CppClass *castClass;
    Il2CppClass *declaringType;
    Il2CppClass *parent;
    Il2CppGenericClass *generic_class;
    Il2CppMetadataTypeHandle typeMetadataHandle;
    const Il2CppInteropData *interopData;
    Il2CppClass *klass;
    FieldInfo *fields;
    const EventInfo *events;
    const PropertyInfo *properties;
    const MethodInfo **methods;
    Il2CppClass **nestedTypes;
    Il2CppClass **implementedInterfaces;
    Il2CppRuntimeInterfaceOffsetPair *interfaceOffsets;
    void *static_fields;
    const Il2CppRGCTXData *rgctx_data;
    struct Il2CppClass **typeHierarchy;
    void *unity_user_data;
    Il2CppGCHandle initializationExceptionGCHandle;
    uint32_t cctor_started;
    uint32_t cctor_finished_or_no_cctor;
    __attribute__((aligned(8))) size_t cctor_thread;
    Il2CppMetadataGenericContainerHandle genericContainerHandle;
    uint32_t instance_size;
    uint32_t stack_slot_size;
    uint32_t actualSize;
    uint32_t element_size;
    int32_t native_size;
    uint32_t static_fields_size;
    uint32_t thread_static_fields_size;
    int32_t thread_static_fields_offset;
    uint32_t flags;
    uint32_t token;
    uint16_t method_count;
    uint16_t property_count;
    uint16_t field_count;
    uint16_t event_count;
    uint16_t nested_type_count;
    uint16_t vtable_count;
    uint16_t interfaces_count;
    uint16_t interface_offsets_count;
    uint8_t typeHierarchyDepth;
    uint8_t genericRecursionDepth;
    uint8_t rank;
    uint8_t minimumAlignment;
    uint8_t packingSize;
    uint8_t initialized_and_no_error : 1;
    uint8_t initialized : 1;
    uint8_t enumtype : 1;
    uint8_t nullabletype : 1;
    uint8_t is_generic : 1;
    uint8_t has_references : 1;
    uint8_t init_pending : 1;
    uint8_t size_init_pending : 1;
    uint8_t size_inited : 1;
    uint8_t has_finalize : 1;
    uint8_t has_cctor : 1;
    uint8_t is_blittable : 1;
    uint8_t is_import_or_windows_runtime : 1;
    uint8_t is_vtable_initialized : 1;
    uint8_t is_byref_like : 1;
    VirtualInvokeData vtable[0];
} Il2CppClass;

typedef struct FieldInfo {
    const char *name;
    const Il2CppType *type;
    Il2CppClass *parent;
    int32_t offset;
    uint32_t token;
} FieldInfo;

typedef struct PropertyInfo {
    Il2CppClass *parent;
    const char *name;
    const MethodInfo *get;
    const MethodInfo *set;
    uint32_t attrs;
    uint32_t token;
} PropertyInfo;

typedef struct EventInfo {
    const char *name;
    const Il2CppType *eventType;
    Il2CppClass *parent;
    const MethodInfo *add;
    const MethodInfo *remove;
    const MethodInfo *raise;
    uint32_t token;
} EventInfo;

typedef void (*InvokerMethod)(Il2CppMethodPointer, const MethodInfo *, void *, void **, void *);

typedef struct MethodInfo {
    Il2CppMethodPointer methodPointer;
    Il2CppMethodPointer virtualMethodPointer;
    InvokerMethod invoker_method;
    const char *name;
    Il2CppClass *klass;
    const Il2CppType *return_type;
    const Il2CppType **parameters;
    union {
        const Il2CppRGCTXData *rgctx_data;
        Il2CppMetadataMethodDefinitionHandle methodMetadataHandle;
    };
    union {
        const Il2CppGenericMethod *genericMethod;
        Il2CppMetadataGenericContainerHandle genericContainerHandle;
    };
    uint32_t token;
    uint16_t flags;
    uint16_t iflags;
    uint16_t slot;
    uint8_t parameters_count;
    uint8_t is_generic : 1;
    uint8_t is_inflated : 1;
    uint8_t wrapper_type : 1;
    uint8_t has_full_generic_sharing_signature : 1;
} MethodInfo;

typedef struct Il2CppAssemblyName {
    const char *name;
    const char *culture;
    const uint8_t *public_key;
    uint32_t hash_alg;
    int32_t hash_len;
    uint32_t flags;
    int32_t major;
    int32_t minor;
    int32_t build;
    int32_t revision;
    uint8_t public_key_token[8];
} Il2CppAssemblyName;

typedef struct Il2CppImage {
    const char *name;
    const char *nameNoExt;
    Il2CppAssembly *assembly;
    uint32_t typeCount;
    uint32_t exportedTypeCount;
    uint32_t customAttributeCount;
    Il2CppMetadataImageHandle metadataHandle;
    Il2CppNameToTypeHandleHashTable *nameToClassHashTable;
    const Il2CppCodeGenModule *codeGenModule;
    uint32_t token;
    uint8_t dynamic;
} Il2CppImage;

typedef struct Il2CppAssembly {
    Il2CppImage *image;
    uint32_t token;
    int32_t referencedAssemblyStart;
    int32_t referencedAssemblyCount;
    Il2CppAssemblyName aname;
} Il2CppAssembly;

typedef struct Il2CppDomain {
    Il2CppAppDomain *domain;
    Il2CppAppDomainSetup *setup;
    Il2CppAppContext *default_context;
    Il2CppObject *ephemeron_tombstone;
    const char *friendly_name;
    uint32_t domain_id;
    volatile int threadpool_jobs;
    void *agent_info;
} Il2CppDomain;

typedef struct Il2CppThreadName {
    IL2CPP::Il2CppChar *chars;
    int32_t unused;
    int32_t length;
} Il2CppThreadName;

typedef struct Il2CppInternalThread {
    Il2CppObject obj;
    int lock_thread_id;
    void *handle;
    void *native_handle;
    Il2CppThreadName name;
    uint32_t state;
    Il2CppObject *abort_exc;
    int abort_state_handle;
    uint64_t tid;
    intptr_t debugger_thread;
    void **static_data;
    void *runtime_thread_info;
    Il2CppObject *current_appcontext;
    Il2CppObject *root_domain_thread;
    Il2CppArray *_serialized_principal;
    int _serialized_principal_version;
    void *appdomain_refs;
    int32_t interruption_requested;
    void *longlived;
    uint8_t threadpool_thread;
    uint8_t thread_interrupt_requested;
} Il2CppInternalThread;

typedef struct Il2CppThread {
    Il2CppObject obj;
    Il2CppInternalThread *internal_thread;
    Il2CppObject *start_obj;
    Il2CppException *pending_exception;
    Il2CppObject *principal;
    int32_t principal_version;
    Il2CppDelegate *delegate;
    Il2CppObject *executionContext;
    uint8_t executionContextBelongsToOuterScope;
} Il2CppThread;

typedef struct Il2CppException {
    Il2CppObject object;
    Il2CppString *className;
    Il2CppString *message;
    Il2CppObject *_data;
    struct Il2CppException *inner_ex;
    Il2CppString *_helpURL;
    Il2CppArray *trace_ips;
    Il2CppString *stack_trace;
    Il2CppString *remote_stack_trace;
    int remote_stack_index;
    Il2CppObject *_dynamicMethods;
    il2cpp_hresult_t hresult;
    Il2CppString *source;
    Il2CppObject *safeSerializationManager;
    Il2CppArray *captured_traces;
    Il2CppArray *native_trace_ips;
} Il2CppException;

}
}
