#pragma once

#include "Debug.hpp"
#include "Il2CppStructures.hpp"
namespace BNM {
typedef IL2CPP::Il2CppReflectionType MonoType;

struct Class;
struct MethodBase;
struct FieldBase;
struct PropertyBase;
struct EventBase;
struct Image;
struct CompileTimeClass;
struct DelegateBase;
struct MulticastDelegateBase;

namespace PRIVATE_INTERNAL {
    template<typename Ret>
    inline Ret ReturnEmpty() { return {}; }

    inline IL2CPP::Il2CppClass *&GetMethodClass(IL2CPP::MethodInfo *method) { return method->klass; }
}

void EmptyMethod();
void *OffsetInLib(void *offsetInMemory);
bool CheckHandle(void *handle);

namespace Internal {
void ResolveApi();
IL2CPP::Il2CppClass *TryGetClassWithoutImage(const std::string_view &_namespace, const std::string_view &_name);
namespace AssemblerUtils {
    BNM_PTR FindNextJump(BNM_PTR start, uint8_t index);
    void *FindJump(void *start, uint8_t count);
}

struct States {
    uint8_t state : 1{};
    uint8_t lateInitAllowed : 1{};
};

#define BNM_DO_API(r, n, p) r(*n) p;
struct Il2CppApi {
    BNM_DO_API(int, il2cpp_init, (const char *domain_name));
    BNM_DO_API(int, il2cpp_init_utf16, (const IL2CPP::Il2CppChar *domain_name));
    BNM_DO_API(void, il2cpp_shutdown, ());
    BNM_DO_API(void, il2cpp_set_config_dir, (const char *config_path));
    BNM_DO_API(void, il2cpp_set_data_dir, (const char *data_path));
    BNM_DO_API(void, il2cpp_set_temp_dir, (const char *temp_path));
    BNM_DO_API(void, il2cpp_set_commandline_arguments, (int argc, const char *const argv[], const char *basedir));
    BNM_DO_API(void, il2cpp_set_commandline_arguments_utf16, (int argc, const IL2CPP::Il2CppChar *const argv[], const char *basedir));
    BNM_DO_API(void, il2cpp_set_config_utf16, (const IL2CPP::Il2CppChar *executablePath));
    BNM_DO_API(void, il2cpp_set_config, (const char *executablePath));
    BNM_DO_API(void, il2cpp_set_memory_callbacks, (IL2CPP::Il2CppMemoryCallbacks *callbacks));
    BNM_DO_API(IL2CPP::Il2CppImage *, il2cpp_get_corlib, ());
    BNM_DO_API(void, il2cpp_add_internal_call, (const char *name, IL2CPP::Il2CppMethodPointer method));
    BNM_DO_API(IL2CPP::Il2CppMethodPointer, il2cpp_resolve_icall, (const char *name));
    BNM_DO_API(void *, il2cpp_alloc, (size_t size));
    BNM_DO_API(void, il2cpp_free, (void *ptr));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_array_class_get, (IL2CPP::Il2CppClass *element_class, uint32_t rank));
    BNM_DO_API(uint32_t, il2cpp_array_length, (IL2CPP::Il2CppArray *array));
    BNM_DO_API(uint32_t, il2cpp_array_get_byte_length, (IL2CPP::Il2CppArray *array));
    BNM_DO_API(IL2CPP::Il2CppArray *, il2cpp_array_new, (IL2CPP::Il2CppClass *elementTypeInfo, IL2CPP::il2cpp_array_size_t length));
    BNM_DO_API(IL2CPP::Il2CppArray *, il2cpp_array_new_specific, (IL2CPP::Il2CppClass *arrayTypeInfo, IL2CPP::il2cpp_array_size_t length));
    BNM_DO_API(IL2CPP::Il2CppArray *, il2cpp_array_new_full, (IL2CPP::Il2CppClass *array_class, IL2CPP::il2cpp_array_size_t *lengths, IL2CPP::il2cpp_array_size_t *lower_bounds));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_bounded_array_class_get, (IL2CPP::Il2CppClass *element_class, uint32_t rank, bool bounded));
    BNM_DO_API(int, il2cpp_array_element_size, (IL2CPP::Il2CppClass *array_class));
    BNM_DO_API(IL2CPP::Il2CppImage *, il2cpp_assembly_get_image, (IL2CPP::Il2CppAssembly *assembly));
    BNM_DO_API(void, il2cpp_class_for_each, (void(*klassReportFunc)(IL2CPP::Il2CppClass *klass, void *userData), void *userData));
    BNM_DO_API(IL2CPP::Il2CppType *, il2cpp_class_enum_basetype, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(bool, il2cpp_class_is_generic, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(bool, il2cpp_class_is_inflated, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(bool, il2cpp_class_is_assignable_from, (IL2CPP::Il2CppClass *klass, IL2CPP::Il2CppClass *oklass));
    BNM_DO_API(bool, il2cpp_class_is_subclass_of, (IL2CPP::Il2CppClass *klass, IL2CPP::Il2CppClass *klassc, bool check_interfaces));
    BNM_DO_API(bool, il2cpp_class_has_parent, (IL2CPP::Il2CppClass *klass, IL2CPP::Il2CppClass *klassc));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_class_from_il2cpp_type, (IL2CPP::Il2CppType *type));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_class_from_name, (IL2CPP::Il2CppImage *image, const char *namespaze, const char *name));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_class_from_system_type, (IL2CPP::Il2CppReflectionType *type));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_class_get_element_class, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(IL2CPP::EventInfo *, il2cpp_class_get_events, (IL2CPP::Il2CppClass *klass, void **iter));
    BNM_DO_API(IL2CPP::FieldInfo *, il2cpp_class_get_fields, (IL2CPP::Il2CppClass *klass, void **iter));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_class_get_nested_types, (IL2CPP::Il2CppClass *klass, void **iter));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_class_get_interfaces, (IL2CPP::Il2CppClass *klass, void **iter));
    BNM_DO_API(IL2CPP::PropertyInfo *, il2cpp_class_get_properties, (IL2CPP::Il2CppClass *klass, void **iter));
    BNM_DO_API(IL2CPP::PropertyInfo *, il2cpp_class_get_property_from_name, (IL2CPP::Il2CppClass *klass, const char *name));
    BNM_DO_API(IL2CPP::FieldInfo *, il2cpp_class_get_field_from_name, (IL2CPP::Il2CppClass *klass, const char *name));
    BNM_DO_API(IL2CPP::MethodInfo *, il2cpp_class_get_methods, (IL2CPP::Il2CppClass *klass, void **iter));
    BNM_DO_API(IL2CPP::MethodInfo *, il2cpp_class_get_method_from_name, (IL2CPP::Il2CppClass *klass, const char *name, int argsCount));
    BNM_DO_API(const char *, il2cpp_class_get_name, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(void, il2cpp_type_get_name_chunked, (IL2CPP::Il2CppType *type, void(*chunkReportFunc)(void *data, void *userData), void *userData));
    BNM_DO_API(char *, il2cpp_class_get_namespace, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_class_get_parent, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_class_get_declaring_type, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(int32_t, il2cpp_class_instance_size, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(size_t, il2cpp_class_num_fields, (IL2CPP::Il2CppClass *enumKlass));
    BNM_DO_API(bool, il2cpp_class_is_valuetype, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(int32_t, il2cpp_class_value_size, (IL2CPP::Il2CppClass *klass, uint32_t *align));
    BNM_DO_API(bool, il2cpp_class_is_blittable, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(int, il2cpp_class_get_flags, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(bool, il2cpp_class_is_abstract, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(bool, il2cpp_class_is_interface, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(int, il2cpp_class_array_element_size, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_class_from_type, (IL2CPP::Il2CppType *type));
    BNM_DO_API(IL2CPP::Il2CppType *, il2cpp_class_get_type, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(uint32_t, il2cpp_class_get_type_token, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(bool, il2cpp_class_has_attribute, (IL2CPP::Il2CppClass *klass, IL2CPP::Il2CppClass *attr_class));
    BNM_DO_API(bool, il2cpp_class_has_references, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(bool, il2cpp_class_is_enum, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(IL2CPP::Il2CppImage *, il2cpp_class_get_image, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(const char *, il2cpp_class_get_assemblyname, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(int, il2cpp_class_get_rank, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(uint32_t, il2cpp_class_get_data_size, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(void *, il2cpp_class_get_static_field_data, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(size_t, il2cpp_class_get_bitmap_size, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(void, il2cpp_class_get_bitmap, (IL2CPP::Il2CppClass *klass, size_t *bitmap));
    BNM_DO_API(bool, il2cpp_stats_dump_to_file, (const char *path));
    BNM_DO_API(uint64_t, il2cpp_stats_get_value, (IL2CPP::Il2CppStat stat));
    BNM_DO_API(IL2CPP::Il2CppDomain *, il2cpp_domain_get, ());
    BNM_DO_API(IL2CPP::Il2CppAssembly *, il2cpp_domain_assembly_open, (IL2CPP::Il2CppDomain *domain, const char *name));
    BNM_DO_API(IL2CPP::Il2CppAssembly **, il2cpp_domain_get_assemblies, (IL2CPP::Il2CppDomain *domain, size_t *size));
    BNM_DO_API(void, il2cpp_raise_exception, (IL2CPP::Il2CppException *));
    BNM_DO_API(IL2CPP::Il2CppException *, il2cpp_exception_from_name_msg, (IL2CPP::Il2CppImage *image, const char *name_space, const char *name, const char *msg));
    BNM_DO_API(IL2CPP::Il2CppException *, il2cpp_get_exception_argument_null, (char *arg));
    BNM_DO_API(void, il2cpp_format_exception, (IL2CPP::Il2CppException *ex, char *message, int message_size));
    BNM_DO_API(void, il2cpp_format_stack_trace, (IL2CPP::Il2CppException *ex, char *output, int output_size));
    BNM_DO_API(void, il2cpp_unhandled_exception, (IL2CPP::Il2CppException *));
    BNM_DO_API(void, il2cpp_native_stack_trace, (IL2CPP::Il2CppException *ex, uintptr_t **addresses, int *numFrames, char **imageUUID, char **imageName));
    BNM_DO_API(int, il2cpp_field_get_flags, (IL2CPP::FieldInfo *field));
    BNM_DO_API(const char *, il2cpp_field_get_name, (IL2CPP::FieldInfo *field));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_field_get_parent, (IL2CPP::FieldInfo *field));
    BNM_DO_API(size_t, il2cpp_field_get_offset, (IL2CPP::FieldInfo *field));
    BNM_DO_API(IL2CPP::Il2CppType *, il2cpp_field_get_type, (IL2CPP::FieldInfo *field));
    BNM_DO_API(void, il2cpp_field_get_value, (IL2CPP::Il2CppObject *obj, IL2CPP::FieldInfo *field, void *value));
    BNM_DO_API(IL2CPP::Il2CppObject *, il2cpp_field_get_value_object, (IL2CPP::FieldInfo *field, IL2CPP::Il2CppObject *obj));
    BNM_DO_API(bool, il2cpp_field_has_attribute, (IL2CPP::FieldInfo *field, IL2CPP::Il2CppClass *attr_class));
    BNM_DO_API(void, il2cpp_field_set_value, (IL2CPP::Il2CppObject *obj, IL2CPP::FieldInfo *field, void *value));
    BNM_DO_API(void, il2cpp_field_static_get_value, (IL2CPP::FieldInfo *field, void *value));
    BNM_DO_API(void, il2cpp_field_static_set_value, (IL2CPP::FieldInfo *field, void *value));
    BNM_DO_API(void, il2cpp_field_set_value_object, (IL2CPP::Il2CppObject *instance, IL2CPP::FieldInfo *field, IL2CPP::Il2CppObject *value));
    BNM_DO_API(bool, il2cpp_field_is_literal, (IL2CPP::FieldInfo *field));
    BNM_DO_API(void, il2cpp_gc_collect, (int maxGenerations));
    BNM_DO_API(int32_t, il2cpp_gc_collect_a_little, ());
    BNM_DO_API(void, il2cpp_gc_start_incremental_collection, ());
    BNM_DO_API(void, il2cpp_gc_disable, ());
    BNM_DO_API(void, il2cpp_gc_enable, ());
    BNM_DO_API(bool, il2cpp_gc_is_disabled, ());
    BNM_DO_API(void, il2cpp_gc_set_mode, (IL2CPP::Il2CppGCMode mode));
    BNM_DO_API(int64_t, il2cpp_gc_get_max_time_slice_ns, ());
    BNM_DO_API(void, il2cpp_gc_set_max_time_slice_ns, (int64_t maxTimeSlice));
    BNM_DO_API(bool, il2cpp_gc_is_incremental, ());
    BNM_DO_API(int64_t, il2cpp_gc_get_used_size, ());
    BNM_DO_API(int64_t, il2cpp_gc_get_heap_size, ());
    BNM_DO_API(void, il2cpp_gc_wbarrier_set_field, (IL2CPP::Il2CppObject *obj, void **targetAddress, void *object));
    BNM_DO_API(bool, il2cpp_gc_has_strict_wbarriers, ());
    BNM_DO_API(void, il2cpp_gc_set_external_allocation_tracker, (void(*func)(void *, size_t, int)));
    BNM_DO_API(void, il2cpp_gc_set_external_wbarrier_tracker, (void(*func)(void **)));
    BNM_DO_API(void, il2cpp_gc_foreach_heap, (void(*func)(void *data, void *userData), void *userData));
    BNM_DO_API(void, il2cpp_stop_gc_world, ());
    BNM_DO_API(void, il2cpp_start_gc_world, ());
    BNM_DO_API(void *, il2cpp_gc_alloc_fixed, (size_t size));
    BNM_DO_API(void, il2cpp_gc_free_fixed, (void *address));
    BNM_DO_API(uint32_t, il2cpp_gchandle_new, (IL2CPP::Il2CppObject *obj, bool pinned));
    BNM_DO_API(uint32_t, il2cpp_gchandle_new_weakref, (IL2CPP::Il2CppObject *obj, bool track_resurrection));
    BNM_DO_API(IL2CPP::Il2CppObject *, il2cpp_gchandle_get_target, (uint32_t gchandle));
    BNM_DO_API(void, il2cpp_gchandle_free, (uint32_t gchandle));
    BNM_DO_API(void, il2cpp_gchandle_foreach_get_target, (void(*func)(void *data, void *userData), void *userData));
    BNM_DO_API(uint32_t, il2cpp_object_header_size, ());
    BNM_DO_API(uint32_t, il2cpp_array_object_header_size, ());
    BNM_DO_API(uint32_t, il2cpp_offset_of_array_length_in_array_object_header, ());
    BNM_DO_API(uint32_t, il2cpp_offset_of_array_bounds_in_array_object_header, ());
    BNM_DO_API(uint32_t, il2cpp_allocation_granularity, ());
    BNM_DO_API(void *, il2cpp_unity_liveness_allocate_struct, (IL2CPP::Il2CppClass *filter, int max_object_count, IL2CPP::il2cpp_register_object_callback callback, void *userdata, IL2CPP::il2cpp_liveness_reallocate_callback reallocate));
    BNM_DO_API(void, il2cpp_unity_liveness_calculation_from_root, (IL2CPP::Il2CppObject *root, void *state));
    BNM_DO_API(void, il2cpp_unity_liveness_calculation_from_statics, (void *state));
    BNM_DO_API(void, il2cpp_unity_liveness_finalize, (void *state));
    BNM_DO_API(void, il2cpp_unity_liveness_free_struct, (void *state));
    BNM_DO_API(void *, il2cpp_unity_liveness_calculation_begin, (IL2CPP::Il2CppClass *filter, int max_object_count, IL2CPP::il2cpp_register_object_callback callback, void *userdata, IL2CPP::il2cpp_WorldChangedCallback onWorldStarted, IL2CPP::il2cpp_WorldChangedCallback onWorldStopped));
    BNM_DO_API(void, il2cpp_unity_liveness_calculation_end, (void *state));
    BNM_DO_API(IL2CPP::Il2CppType *, il2cpp_method_get_return_type, (IL2CPP::MethodInfo *method));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_method_get_declaring_type, (IL2CPP::MethodInfo *method));
    BNM_DO_API(const char *, il2cpp_method_get_name, (IL2CPP::MethodInfo *method));
    BNM_DO_API(IL2CPP::MethodInfo *, il2cpp_method_get_from_reflection, (IL2CPP::Il2CppReflectionMethod *method));
    BNM_DO_API(IL2CPP::Il2CppReflectionMethod *, il2cpp_method_get_object, (IL2CPP::MethodInfo *method, IL2CPP::Il2CppClass *refclass));
    BNM_DO_API(bool, il2cpp_method_is_generic, (IL2CPP::MethodInfo *method));
    BNM_DO_API(bool, il2cpp_method_is_inflated, (IL2CPP::MethodInfo *method));
    BNM_DO_API(bool, il2cpp_method_is_instance, (IL2CPP::MethodInfo *method));
    BNM_DO_API(uint32_t, il2cpp_method_get_param_count, (IL2CPP::MethodInfo *method));
    BNM_DO_API(IL2CPP::Il2CppType *, il2cpp_method_get_param, (IL2CPP::MethodInfo *method, uint32_t index));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_method_get_class, (IL2CPP::MethodInfo *method));
    BNM_DO_API(bool, il2cpp_method_has_attribute, (IL2CPP::MethodInfo *method, IL2CPP::Il2CppClass *attr_class));
    BNM_DO_API(uint32_t, il2cpp_method_get_flags, (IL2CPP::MethodInfo *method, uint32_t *iflags));
    BNM_DO_API(uint32_t, il2cpp_method_get_token, (IL2CPP::MethodInfo *method));
    BNM_DO_API(const char *, il2cpp_method_get_param_name, (IL2CPP::MethodInfo *method, uint32_t index));
    BNM_DO_API(uint32_t, il2cpp_property_get_flags, (IL2CPP::PropertyInfo *prop));
    BNM_DO_API(IL2CPP::MethodInfo *, il2cpp_property_get_get_method, (IL2CPP::PropertyInfo *prop));
    BNM_DO_API(IL2CPP::MethodInfo *, il2cpp_property_get_set_method, (IL2CPP::PropertyInfo *prop));
    BNM_DO_API(const char *, il2cpp_property_get_name, (IL2CPP::PropertyInfo *prop));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_property_get_parent, (IL2CPP::PropertyInfo *prop));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_object_get_class, (IL2CPP::Il2CppObject *obj));
    BNM_DO_API(uint32_t, il2cpp_object_get_size, (IL2CPP::Il2CppObject *obj));
    BNM_DO_API(IL2CPP::MethodInfo *, il2cpp_object_get_virtual_method, (IL2CPP::Il2CppObject *obj, IL2CPP::MethodInfo *method));
    BNM_DO_API(IL2CPP::Il2CppObject *, il2cpp_object_new, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(void *, il2cpp_object_unbox, (IL2CPP::Il2CppObject *obj));
    BNM_DO_API(IL2CPP::Il2CppObject *, il2cpp_value_box, (IL2CPP::Il2CppClass *klass, void *data));
    BNM_DO_API(void, il2cpp_monitor_enter, (IL2CPP::Il2CppObject *obj));
    BNM_DO_API(bool, il2cpp_monitor_try_enter, (IL2CPP::Il2CppObject *obj, uint32_t timeout));
    BNM_DO_API(void, il2cpp_monitor_exit, (IL2CPP::Il2CppObject *obj));
    BNM_DO_API(void, il2cpp_monitor_pulse, (IL2CPP::Il2CppObject *obj));
    BNM_DO_API(void, il2cpp_monitor_pulse_all, (IL2CPP::Il2CppObject *obj));
    BNM_DO_API(void, il2cpp_monitor_wait, (IL2CPP::Il2CppObject *obj));
    BNM_DO_API(bool, il2cpp_monitor_try_wait, (IL2CPP::Il2CppObject *obj, uint32_t timeout));
    BNM_DO_API(IL2CPP::Il2CppObject *, il2cpp_runtime_invoke, (IL2CPP::MethodInfo *method, void *obj, void **params, IL2CPP::Il2CppException **exc));
    BNM_DO_API(IL2CPP::Il2CppObject *, il2cpp_runtime_invoke_convert_args, (IL2CPP::MethodInfo *method, void *obj, IL2CPP::Il2CppObject **params, int paramCount, IL2CPP::Il2CppException **exc));
    BNM_DO_API(void, il2cpp_runtime_class_init, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(void, il2cpp_runtime_object_init, (IL2CPP::Il2CppObject *obj));
    BNM_DO_API(void, il2cpp_runtime_object_init_exception, (IL2CPP::Il2CppObject *obj, IL2CPP::Il2CppException **exc));
    BNM_DO_API(void, il2cpp_runtime_unhandled_exception_policy_set, (IL2CPP::Il2CppRuntimeUnhandledExceptionPolicy value));
    BNM_DO_API(int32_t, il2cpp_string_length, (IL2CPP::Il2CppString *str));
    BNM_DO_API(IL2CPP::Il2CppChar *, il2cpp_string_chars, (IL2CPP::Il2CppString *str));
    BNM_DO_API(IL2CPP::Il2CppString *, il2cpp_string_new, (const char *str));
    BNM_DO_API(IL2CPP::Il2CppString *, il2cpp_string_new_len, (const char *str, uint32_t length));
    BNM_DO_API(IL2CPP::Il2CppString *, il2cpp_string_new_utf16, (const IL2CPP::Il2CppChar *text, int32_t len));
    BNM_DO_API(IL2CPP::Il2CppString *, il2cpp_string_new_wrapper, (const char *str));
    BNM_DO_API(IL2CPP::Il2CppString *, il2cpp_string_intern, (IL2CPP::Il2CppString *str));
    BNM_DO_API(IL2CPP::Il2CppString *, il2cpp_string_is_interned, (IL2CPP::Il2CppString *str));
    BNM_DO_API(IL2CPP::Il2CppThread *, il2cpp_thread_current, ());
    BNM_DO_API(IL2CPP::Il2CppThread *, il2cpp_thread_attach, (IL2CPP::Il2CppDomain *domain));
    BNM_DO_API(void, il2cpp_thread_detach, (IL2CPP::Il2CppThread *thread));
    BNM_DO_API(IL2CPP::Il2CppThread **, il2cpp_thread_get_all_attached_threads, (size_t *size));
    BNM_DO_API(bool, il2cpp_is_vm_thread, (IL2CPP::Il2CppThread *thread));
    BNM_DO_API(void, il2cpp_current_thread_walk_frame_stack, (IL2CPP::Il2CppFrameWalkFunc func, void *user_data));
    BNM_DO_API(void, il2cpp_thread_walk_frame_stack, (IL2CPP::Il2CppThread *thread, IL2CPP::Il2CppFrameWalkFunc func, void *user_data));
    BNM_DO_API(bool, il2cpp_current_thread_get_top_frame, (IL2CPP::Il2CppStackFrameInfo *frame));
    BNM_DO_API(bool, il2cpp_thread_get_top_frame, (IL2CPP::Il2CppThread *thread, IL2CPP::Il2CppStackFrameInfo *frame));
    BNM_DO_API(bool, il2cpp_current_thread_get_frame_at, (int32_t offset, IL2CPP::Il2CppStackFrameInfo *frame));
    BNM_DO_API(bool, il2cpp_thread_get_frame_at, (IL2CPP::Il2CppThread *thread, int32_t offset, IL2CPP::Il2CppStackFrameInfo *frame));
    BNM_DO_API(int32_t, il2cpp_current_thread_get_stack_depth, ());
    BNM_DO_API(int32_t, il2cpp_thread_get_stack_depth, (IL2CPP::Il2CppThread *thread));
    BNM_DO_API(void, il2cpp_override_stack_backtrace, (IL2CPP::Il2CppBacktraceFunc stackBacktraceFunc));
    BNM_DO_API(IL2CPP::Il2CppObject *, il2cpp_type_get_object, (IL2CPP::Il2CppType *type));
    BNM_DO_API(int, il2cpp_type_get_type, (IL2CPP::Il2CppType *type));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_type_get_class_or_element_class, (IL2CPP::Il2CppType *type));
    BNM_DO_API(const char *, il2cpp_type_get_name, (IL2CPP::Il2CppType *type));
    BNM_DO_API(bool, il2cpp_type_is_byref, (IL2CPP::Il2CppType *type));
    BNM_DO_API(uint32_t, il2cpp_type_get_attrs, (IL2CPP::Il2CppType *type));
    BNM_DO_API(bool, il2cpp_type_equals, (IL2CPP::Il2CppType *type, IL2CPP::Il2CppType *otherType));
    BNM_DO_API(const char *, il2cpp_type_get_assembly_qualified_name, (IL2CPP::Il2CppType *type));
    BNM_DO_API(bool, il2cpp_type_is_static, (IL2CPP::Il2CppType *type));
    BNM_DO_API(bool, il2cpp_type_is_pointer_type, (IL2CPP::Il2CppType *type));
    BNM_DO_API(IL2CPP::Il2CppAssembly *, il2cpp_image_get_assembly, (IL2CPP::Il2CppImage *image));
    BNM_DO_API(const char *, il2cpp_image_get_name, (IL2CPP::Il2CppImage *image));
    BNM_DO_API(const char *, il2cpp_image_get_filename, (IL2CPP::Il2CppImage *image));
    BNM_DO_API(IL2CPP::MethodInfo *, il2cpp_image_get_entry_point, (IL2CPP::Il2CppImage *image));
    BNM_DO_API(size_t, il2cpp_image_get_class_count, (IL2CPP::Il2CppImage *image));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_image_get_class, (IL2CPP::Il2CppImage *image, size_t index));
    BNM_DO_API(void, il2cpp_image_get_types, (IL2CPP::Il2CppImage *image, bool exportedOnly, std::vector<IL2CPP::Il2CppClass *> *types));
    BNM_DO_API(IL2CPP::Il2CppManagedMemorySnapshot *, il2cpp_capture_memory_snapshot, ());
    BNM_DO_API(void, il2cpp_free_captured_memory_snapshot, (IL2CPP::Il2CppManagedMemorySnapshot *snapshot));
    BNM_DO_API(void, il2cpp_set_find_plugin_callback, (IL2CPP::Il2CppSetFindPlugInCallback method));
    BNM_DO_API(void, il2cpp_register_log_callback, (IL2CPP::Il2CppLogCallback method));
    BNM_DO_API(void, il2cpp_debugger_set_agent_options, (const char *options));
    BNM_DO_API(bool, il2cpp_is_debugger_attached, ());
    BNM_DO_API(void, il2cpp_register_debugger_agent_transport, (IL2CPP::Il2CppDebuggerTransport *debuggerTransport));
    BNM_DO_API(bool, il2cpp_debug_get_method_info, (IL2CPP::MethodInfo *, IL2CPP::Il2CppMethodDebugInfo *methodDebugInfo));
    BNM_DO_API(void, il2cpp_unity_install_unitytls_interface, (void *unitytlsInterfaceStruct));
    BNM_DO_API(IL2CPP::Il2CppCustomAttrInfo *, il2cpp_custom_attrs_from_class, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(IL2CPP::Il2CppCustomAttrInfo *, il2cpp_custom_attrs_from_method, (IL2CPP::MethodInfo *method));
    BNM_DO_API(IL2CPP::Il2CppObject *, il2cpp_custom_attrs_get_attr, (IL2CPP::Il2CppCustomAttrInfo *ainfo, IL2CPP::Il2CppClass *attr_klass));
    BNM_DO_API(bool, il2cpp_custom_attrs_has_attr, (IL2CPP::Il2CppCustomAttrInfo *ainfo, IL2CPP::Il2CppClass *attr_klass));
    BNM_DO_API(IL2CPP::Il2CppArray *, il2cpp_custom_attrs_construct, (IL2CPP::Il2CppCustomAttrInfo *cinfo));
    BNM_DO_API(void, il2cpp_custom_attrs_free, (IL2CPP::Il2CppCustomAttrInfo *ainfo));
    BNM_DO_API(void, il2cpp_class_set_userdata, (IL2CPP::Il2CppClass *klass, void *userdata));
    BNM_DO_API(int, il2cpp_class_get_userdata_offset, ());
    BNM_DO_API(void, il2cpp_set_default_thread_affinity, (int64_t affinity_mask));
};
#undef BNM_DO_API

extern Il2CppApi api;

extern States states;
extern void *il2cppLibraryHandle;
extern void *currentFinderData;
extern std::vector<IL2CPP::Il2CppAssembly *> assembliesCache;

typedef void *(*MethodFinder)(const char *name, void *userData);
extern MethodFinder currentFinderMethod;

void *BasicFinder(const char *name, void *userData);

IL2CPP::Il2CppImage *TryGetImage(const std::string_view &_name);
IL2CPP::Il2CppClass *TryGetClassInImage(const IL2CPP::Il2CppImage *image, const std::string_view &_namespace, const std::string_view &_name);
Class TryMakeGenericClass(Class genericType, const std::vector<CompileTimeClass> &templateTypes);
MethodBase TryMakeGenericMethod(const MethodBase &genericMethod, const std::vector<CompileTimeClass> &templateTypes);
Class GetPointer(Class target);
Class GetReference(Class target);
void *GetIl2CppMethod(const char *methodName);
void Load();
void SetupBNM();
void LateInit(void *il2cpp_class_from_il2cpp_type_addr);
void LoadDefaults();

template<class CompareMethod>
IL2CPP::MethodInfo *IterateMethods(Class target, CompareMethod compare);

bool CompareImageName(IL2CPP::Il2CppImage *image, const std::string_view &name);
std::vector<IL2CPP::Il2CppAssembly *> &GetAllAssemblies();

int BNM_il2cpp_init(const char *domain_name);
IL2CPP::Il2CppClass *BNM_Class$$FromIl2CppType(IL2CPP::Il2CppReflectionType *type);

extern void *BNM_il2cpp_init_origin;
extern int (*old_BNM_il2cpp_init)(const char *);
extern void *BNM_Class$$FromIl2CppType_origin;
extern IL2CPP::Il2CppClass *(*old_BNM_Class$$FromIl2CppType)(IL2CPP::Il2CppReflectionType *);

extern std::string_view constructorName;
extern IL2CPP::Il2CppClass *customListTemplateClass;
extern std::map<uint32_t, IL2CPP::Il2CppClass *> customListsMap;
extern int32_t finalizerSlot;
extern std::vector<void (*)()> onLoadedEvents;

void Image$$GetTypes(const IL2CPP::Il2CppImage *image, bool exportedOnly, std::vector<IL2CPP::Il2CppClass *> *target);

void SetupCoroutine();
void LoadCoroutine();
const char *GetExceptionTypeName();
extern void (*classInitFunc)(IL2CPP::Il2CppClass *);
void Image$$GetTypes(const IL2CPP::Il2CppImage *image, bool exportedOnly, std::vector<IL2CPP::Il2CppClass *> *target);
extern void (*orig_Image$$GetTypes)(const IL2CPP::Il2CppImage *image, bool exportedOnly, std::vector<IL2CPP::Il2CppClass *> *target);

}

namespace Loading {

void AllowLateInitHook();
bool TryLoadByJNI(JNIEnv *env, jobject context = nullptr);
bool TryLoadByDlfcnHandle(void *handle);
typedef void *(*MethodFinder)(const char *name, void *userData);
void SetMethodFinder(MethodFinder finderMethod, void *userData);
bool TryLoadByUsersFinder();
void TrySetupByUsersFinder();
void AddOnLoadedEvent(void (*event)());
void ClearOnLoadedEvents();

}

namespace PRIVATE_FieldUtils {
    void GetStaticValue(IL2CPP::FieldInfo *info, void *value);
    void SetStaticValue(IL2CPP::FieldInfo *info, void *value);
}

namespace Utils {
    template<typename T>
    bool CheckForNull(T obj) { return (void *) obj; }

    template<typename T>
    struct ForwardList {
        struct Element {
            Element *next{};
            T value{};
        };
        Element *lastElement{};
        inline ForwardList() = default;
        inline ~ForwardList() { Clear(); }
        inline ForwardList(const ForwardList &other) : lastElement(nullptr) {
            if (other.IsEmpty()) return;
            auto currentOther = other.lastElement->next;
            do { Add(currentOther->value); currentOther = currentOther->next; } while (currentOther != other.lastElement->next);
        }
        inline ForwardList &operator=(const ForwardList &other) {
            if (this == &other) return *this;
            Clear();
            if (other.IsEmpty()) return *this;
            auto currentOther = other.lastElement->next;
            do { Add(currentOther->value); currentOther = currentOther->next; } while (currentOther != other.lastElement->next);
            return *this;
        }
        inline bool IsEmpty() const { return lastElement == nullptr; }
        inline void Clear() {
            if (!lastElement) return;
            auto current = lastElement->next;
            while (current != lastElement) {
                auto next = current->next;
                delete current;
                current = next;
            }
            delete lastElement;
            lastElement = nullptr;
        }
        inline void Add(const T &value) {
            auto element = new Element{nullptr, value};
            if (!lastElement) { element->next = element; lastElement = element; return; }
            element->next = lastElement->next;
            lastElement->next = element;
            lastElement = element;
        }
    };

    template<typename T>
    inline bool IsAllocated(T x) {
        if (!x) return false;
        volatile char c = *(volatile char *) x;
        (void) c;
        return true;
    }

    template<typename T, typename = std::enable_if_t<std::is_pointer_v<T>>>
    inline T CheckObj(T obj) {
        if (obj && IsAllocated(obj)) return obj;
        return nullptr;
    }

    template<typename T>
    inline T UnboxObject(T obj) { return (T) (void *) (((char *) obj) + sizeof(IL2CPP::Il2CppObject)); }

    template<typename MET_T, typename PTR_T>
    inline void InitFunc(MET_T &method, PTR_T ptr) {
        method = (MET_T) ptr;
    }

    void *OffsetInLib(void *offsetInMemory);
    void LogCompileTimeClass(const CompileTimeClass &compileTimeClass);
}
}
