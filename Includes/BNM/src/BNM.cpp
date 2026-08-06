#include "BNM.hpp"

#include <cstdio>
#include <cmath>
#include <cxxabi.h>

using namespace BNM;

namespace BNM::Internal {

Il2CppApi api{};
States states{};
void (*classInitFunc)(IL2CPP::Il2CppClass *){};
void *il2cppLibraryHandle{};
void *currentFinderData = &il2cppLibraryHandle;
VMData vmData{};
std::vector<IL2CPP::Il2CppAssembly *> assembliesCache{};
MethodFinder currentFinderMethod = BasicFinder;

void *BNM_il2cpp_init_origin{};
int (*old_BNM_il2cpp_init)(const char *){};
void *BNM_Class$$FromIl2CppType_origin{};
IL2CPP::Il2CppClass *(*old_BNM_Class$$FromIl2CppType)(IL2CPP::Il2CppReflectionType *){};

std::string_view constructorName = ".ctor";
IL2CPP::Il2CppClass *customListTemplateClass{};
std::map<uint32_t, IL2CPP::Il2CppClass *> customListsMap{};
int32_t finalizerSlot = -1;
std::vector<void (*)()> onLoadedEvents{};

void *BasicFinder(const char *name, void *userData) {
    return BNM_dlsym(*(void **) userData, name);
}

void *GetIl2CppMethod(const char *methodName) {
    return currentFinderMethod(methodName, currentFinderData);
}

#define RESOLVE_API(name) api.name = (decltype(api.name)) GetIl2CppMethod(BNM_OBFUSCATE_TMP(#name))

void ResolveApi() {
    auto &api = BNM::Internal::api;
    RESOLVE_API(il2cpp_init);
    RESOLVE_API(il2cpp_init_utf16);
    RESOLVE_API(il2cpp_shutdown);
    RESOLVE_API(il2cpp_set_config_dir);
    RESOLVE_API(il2cpp_set_data_dir);
    RESOLVE_API(il2cpp_set_temp_dir);
    RESOLVE_API(il2cpp_set_commandline_arguments);
    RESOLVE_API(il2cpp_set_commandline_arguments_utf16);
    RESOLVE_API(il2cpp_set_config_utf16);
    RESOLVE_API(il2cpp_set_config);
    RESOLVE_API(il2cpp_set_memory_callbacks);
    RESOLVE_API(il2cpp_get_corlib);
    RESOLVE_API(il2cpp_add_internal_call);
    RESOLVE_API(il2cpp_resolve_icall);
    RESOLVE_API(il2cpp_alloc);
    RESOLVE_API(il2cpp_free);
    RESOLVE_API(il2cpp_array_class_get);
    RESOLVE_API(il2cpp_array_length);
    RESOLVE_API(il2cpp_array_get_byte_length);
    RESOLVE_API(il2cpp_array_new);
    RESOLVE_API(il2cpp_array_new_specific);
    RESOLVE_API(il2cpp_array_new_full);
    RESOLVE_API(il2cpp_bounded_array_class_get);
    RESOLVE_API(il2cpp_array_element_size);
    RESOLVE_API(il2cpp_assembly_get_image);
    RESOLVE_API(il2cpp_class_for_each);
    RESOLVE_API(il2cpp_class_enum_basetype);
    RESOLVE_API(il2cpp_class_is_generic);
    RESOLVE_API(il2cpp_class_is_inflated);
    RESOLVE_API(il2cpp_class_is_assignable_from);
    RESOLVE_API(il2cpp_class_is_subclass_of);
    RESOLVE_API(il2cpp_class_has_parent);
    RESOLVE_API(il2cpp_class_from_il2cpp_type);
    RESOLVE_API(il2cpp_class_from_name);
    RESOLVE_API(il2cpp_class_from_system_type);
    RESOLVE_API(il2cpp_class_get_element_class);
    RESOLVE_API(il2cpp_class_get_events);
    RESOLVE_API(il2cpp_class_get_fields);
    RESOLVE_API(il2cpp_class_get_nested_types);
    RESOLVE_API(il2cpp_class_get_interfaces);
    RESOLVE_API(il2cpp_class_get_properties);
    RESOLVE_API(il2cpp_class_get_property_from_name);
    RESOLVE_API(il2cpp_class_get_field_from_name);
    RESOLVE_API(il2cpp_class_get_methods);
    RESOLVE_API(il2cpp_class_get_method_from_name);
    RESOLVE_API(il2cpp_class_get_name);
    RESOLVE_API(il2cpp_type_get_name_chunked);
    RESOLVE_API(il2cpp_class_get_namespace);
    RESOLVE_API(il2cpp_class_get_parent);
    RESOLVE_API(il2cpp_class_get_declaring_type);
    RESOLVE_API(il2cpp_class_instance_size);
    RESOLVE_API(il2cpp_class_num_fields);
    RESOLVE_API(il2cpp_class_is_valuetype);
    RESOLVE_API(il2cpp_class_value_size);
    RESOLVE_API(il2cpp_class_is_blittable);
    RESOLVE_API(il2cpp_class_get_flags);
    RESOLVE_API(il2cpp_class_is_abstract);
    RESOLVE_API(il2cpp_class_is_interface);
    RESOLVE_API(il2cpp_class_array_element_size);
    RESOLVE_API(il2cpp_class_from_type);
    RESOLVE_API(il2cpp_class_get_type);
    RESOLVE_API(il2cpp_class_get_type_token);
    RESOLVE_API(il2cpp_class_has_attribute);
    RESOLVE_API(il2cpp_class_has_references);
    RESOLVE_API(il2cpp_class_is_enum);
    RESOLVE_API(il2cpp_class_get_image);
    RESOLVE_API(il2cpp_class_get_assemblyname);
    RESOLVE_API(il2cpp_class_get_rank);
    RESOLVE_API(il2cpp_class_get_data_size);
    RESOLVE_API(il2cpp_class_get_static_field_data);
    RESOLVE_API(il2cpp_class_get_bitmap_size);
    RESOLVE_API(il2cpp_class_get_bitmap);
    RESOLVE_API(il2cpp_stats_dump_to_file);
    RESOLVE_API(il2cpp_stats_get_value);
    RESOLVE_API(il2cpp_domain_get);
    RESOLVE_API(il2cpp_domain_assembly_open);
    RESOLVE_API(il2cpp_domain_get_assemblies);
    RESOLVE_API(il2cpp_raise_exception);
    RESOLVE_API(il2cpp_exception_from_name_msg);
    RESOLVE_API(il2cpp_get_exception_argument_null);
    RESOLVE_API(il2cpp_format_exception);
    RESOLVE_API(il2cpp_format_stack_trace);
    RESOLVE_API(il2cpp_unhandled_exception);
    RESOLVE_API(il2cpp_native_stack_trace);
    RESOLVE_API(il2cpp_field_get_flags);
    RESOLVE_API(il2cpp_field_get_name);
    RESOLVE_API(il2cpp_field_get_parent);
    RESOLVE_API(il2cpp_field_get_offset);
    RESOLVE_API(il2cpp_field_get_type);
    RESOLVE_API(il2cpp_field_get_value);
    RESOLVE_API(il2cpp_field_get_value_object);
    RESOLVE_API(il2cpp_field_has_attribute);
    RESOLVE_API(il2cpp_field_set_value);
    RESOLVE_API(il2cpp_field_static_get_value);
    RESOLVE_API(il2cpp_field_static_set_value);
    RESOLVE_API(il2cpp_field_set_value_object);
    RESOLVE_API(il2cpp_field_is_literal);
    RESOLVE_API(il2cpp_gc_collect);
    RESOLVE_API(il2cpp_gc_collect_a_little);
    RESOLVE_API(il2cpp_gc_start_incremental_collection);
    RESOLVE_API(il2cpp_gc_disable);
    RESOLVE_API(il2cpp_gc_enable);
    RESOLVE_API(il2cpp_gc_is_disabled);
    RESOLVE_API(il2cpp_gc_set_mode);
    RESOLVE_API(il2cpp_gc_get_max_time_slice_ns);
    RESOLVE_API(il2cpp_gc_set_max_time_slice_ns);
    RESOLVE_API(il2cpp_gc_is_incremental);
    RESOLVE_API(il2cpp_gc_get_used_size);
    RESOLVE_API(il2cpp_gc_get_heap_size);
    RESOLVE_API(il2cpp_gc_wbarrier_set_field);
    RESOLVE_API(il2cpp_gc_has_strict_wbarriers);
    RESOLVE_API(il2cpp_gc_set_external_allocation_tracker);
    RESOLVE_API(il2cpp_gc_set_external_wbarrier_tracker);
    RESOLVE_API(il2cpp_gc_foreach_heap);
    RESOLVE_API(il2cpp_stop_gc_world);
    RESOLVE_API(il2cpp_start_gc_world);
    RESOLVE_API(il2cpp_gc_alloc_fixed);
    RESOLVE_API(il2cpp_gc_free_fixed);
    RESOLVE_API(il2cpp_gchandle_new);
    RESOLVE_API(il2cpp_gchandle_new_weakref);
    RESOLVE_API(il2cpp_gchandle_get_target);
    RESOLVE_API(il2cpp_gchandle_free);
    RESOLVE_API(il2cpp_gchandle_foreach_get_target);
    RESOLVE_API(il2cpp_object_header_size);
    RESOLVE_API(il2cpp_array_object_header_size);
    RESOLVE_API(il2cpp_offset_of_array_length_in_array_object_header);
    RESOLVE_API(il2cpp_offset_of_array_bounds_in_array_object_header);
    RESOLVE_API(il2cpp_allocation_granularity);
    RESOLVE_API(il2cpp_unity_liveness_allocate_struct);
    RESOLVE_API(il2cpp_unity_liveness_calculation_from_root);
    RESOLVE_API(il2cpp_unity_liveness_calculation_from_statics);
    RESOLVE_API(il2cpp_unity_liveness_finalize);
    RESOLVE_API(il2cpp_unity_liveness_free_struct);
    RESOLVE_API(il2cpp_unity_liveness_calculation_begin);
    RESOLVE_API(il2cpp_unity_liveness_calculation_end);
    RESOLVE_API(il2cpp_method_get_return_type);
    RESOLVE_API(il2cpp_method_get_declaring_type);
    RESOLVE_API(il2cpp_method_get_name);
    RESOLVE_API(il2cpp_method_get_from_reflection);
    RESOLVE_API(il2cpp_method_get_object);
    RESOLVE_API(il2cpp_method_is_generic);
    RESOLVE_API(il2cpp_method_is_inflated);
    RESOLVE_API(il2cpp_method_is_instance);
    RESOLVE_API(il2cpp_method_get_param_count);
    RESOLVE_API(il2cpp_method_get_param);
    RESOLVE_API(il2cpp_method_get_class);
    RESOLVE_API(il2cpp_method_has_attribute);
    RESOLVE_API(il2cpp_method_get_flags);
    RESOLVE_API(il2cpp_method_get_token);
    RESOLVE_API(il2cpp_method_get_param_name);
    RESOLVE_API(il2cpp_property_get_flags);
    RESOLVE_API(il2cpp_property_get_get_method);
    RESOLVE_API(il2cpp_property_get_set_method);
    RESOLVE_API(il2cpp_property_get_name);
    RESOLVE_API(il2cpp_property_get_parent);
    RESOLVE_API(il2cpp_object_get_class);
    RESOLVE_API(il2cpp_object_get_size);
    RESOLVE_API(il2cpp_object_get_virtual_method);
    RESOLVE_API(il2cpp_object_new);
    RESOLVE_API(il2cpp_object_unbox);
    RESOLVE_API(il2cpp_value_box);
    RESOLVE_API(il2cpp_monitor_enter);
    RESOLVE_API(il2cpp_monitor_try_enter);
    RESOLVE_API(il2cpp_monitor_exit);
    RESOLVE_API(il2cpp_monitor_pulse);
    RESOLVE_API(il2cpp_monitor_pulse_all);
    RESOLVE_API(il2cpp_monitor_wait);
    RESOLVE_API(il2cpp_monitor_try_wait);
    RESOLVE_API(il2cpp_runtime_invoke);
    RESOLVE_API(il2cpp_runtime_invoke_convert_args);
    RESOLVE_API(il2cpp_runtime_class_init);
    RESOLVE_API(il2cpp_runtime_object_init);
    RESOLVE_API(il2cpp_runtime_object_init_exception);
    RESOLVE_API(il2cpp_runtime_unhandled_exception_policy_set);
    RESOLVE_API(il2cpp_string_length);
    RESOLVE_API(il2cpp_string_chars);
    RESOLVE_API(il2cpp_string_new);
    RESOLVE_API(il2cpp_string_new_len);
    RESOLVE_API(il2cpp_string_new_utf16);
    RESOLVE_API(il2cpp_string_new_wrapper);
    RESOLVE_API(il2cpp_string_intern);
    RESOLVE_API(il2cpp_string_is_interned);
    RESOLVE_API(il2cpp_thread_current);
    RESOLVE_API(il2cpp_thread_attach);
    RESOLVE_API(il2cpp_thread_detach);
    RESOLVE_API(il2cpp_thread_get_all_attached_threads);
    RESOLVE_API(il2cpp_is_vm_thread);
    RESOLVE_API(il2cpp_current_thread_walk_frame_stack);
    RESOLVE_API(il2cpp_thread_walk_frame_stack);
    RESOLVE_API(il2cpp_current_thread_get_top_frame);
    RESOLVE_API(il2cpp_thread_get_top_frame);
    RESOLVE_API(il2cpp_current_thread_get_frame_at);
    RESOLVE_API(il2cpp_thread_get_frame_at);
    RESOLVE_API(il2cpp_current_thread_get_stack_depth);
    RESOLVE_API(il2cpp_thread_get_stack_depth);
    RESOLVE_API(il2cpp_override_stack_backtrace);
    RESOLVE_API(il2cpp_type_get_object);
    RESOLVE_API(il2cpp_type_get_type);
    RESOLVE_API(il2cpp_type_get_class_or_element_class);
    RESOLVE_API(il2cpp_type_get_name);
    RESOLVE_API(il2cpp_type_is_byref);
    RESOLVE_API(il2cpp_type_get_attrs);
    RESOLVE_API(il2cpp_type_equals);
    RESOLVE_API(il2cpp_type_get_assembly_qualified_name);
    RESOLVE_API(il2cpp_type_is_static);
    RESOLVE_API(il2cpp_type_is_pointer_type);
    RESOLVE_API(il2cpp_image_get_assembly);
    RESOLVE_API(il2cpp_image_get_name);
    RESOLVE_API(il2cpp_image_get_filename);
    RESOLVE_API(il2cpp_image_get_entry_point);
    RESOLVE_API(il2cpp_image_get_class_count);
    RESOLVE_API(il2cpp_image_get_class);
    RESOLVE_API(il2cpp_capture_memory_snapshot);
    RESOLVE_API(il2cpp_free_captured_memory_snapshot);
    RESOLVE_API(il2cpp_set_find_plugin_callback);
    RESOLVE_API(il2cpp_register_log_callback);
    RESOLVE_API(il2cpp_debugger_set_agent_options);
    RESOLVE_API(il2cpp_is_debugger_attached);
    RESOLVE_API(il2cpp_register_debugger_agent_transport);
    RESOLVE_API(il2cpp_debug_get_method_info);
    RESOLVE_API(il2cpp_unity_install_unitytls_interface);
    RESOLVE_API(il2cpp_custom_attrs_from_class);
    RESOLVE_API(il2cpp_custom_attrs_from_method);
    RESOLVE_API(il2cpp_custom_attrs_get_attr);
    RESOLVE_API(il2cpp_custom_attrs_has_attr);
    RESOLVE_API(il2cpp_custom_attrs_construct);
    RESOLVE_API(il2cpp_custom_attrs_free);
    RESOLVE_API(il2cpp_class_set_userdata);
    RESOLVE_API(il2cpp_class_get_userdata_offset);
    RESOLVE_API(il2cpp_set_default_thread_affinity);
}

#undef RESOLVE_API

}

void *BNM::GetExternMethod(const std::string_view &str) {
    auto ret = Internal::api.il2cpp_resolve_icall(str.data());
    BNM_LOG_WARN_IF(!ret, "GetExternMethod failed: %s", str.data());
    return (void *) ret;
}

template<>
bool BNM::IsA<BNM::IL2CPP::Il2CppObject *>(BNM::IL2CPP::Il2CppObject *object, BNM::IL2CPP::Il2CppClass *_class) {
    if (!object || !_class) return false;
    for (auto cls = object->klass; cls; cls = cls->parent) if (cls == _class) return true;
    return false;
}

bool BNM::IsLoaded() {
    return Internal::states.state;
}

void *BNM::GetIl2CppLibraryHandle() {
    return Internal::il2cppLibraryHandle;
}

bool BNM::AttachIl2Cpp() {
    if (CurrentIl2CppThread()) return false;
    Internal::api.il2cpp_thread_attach(Internal::api.il2cpp_domain_get());
    return true;
}

IL2CPP::Il2CppThread *BNM::CurrentIl2CppThread() {
    return Internal::api.il2cpp_thread_current();
}

void BNM::DetachIl2Cpp() {
    auto thread = CurrentIl2CppThread();
    if (!thread) return;
    Internal::api.il2cpp_thread_detach(thread);
}

void *BNM::Allocate(size_t size) {
    return Internal::api.il2cpp_gc_alloc_fixed(size);
}

void BNM::Free(void *ptr) {
    return Internal::api.il2cpp_gc_free_fixed(ptr);
}
