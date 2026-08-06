#include "BNM.hpp"

using namespace BNM;

Image::Image(const std::string_view &name) {
    _data = Internal::TryGetImage(name);

    BNM_LOG_WARN_IF(!_data, "Image not found: %s", name.data());
}

Image::Image(const BNM::IL2CPP::Il2CppAssembly *assembly) {
    _data = Internal::api.il2cpp_assembly_get_image((IL2CPP::Il2CppAssembly *) assembly);
}

std::vector<BNM::Class> Image::GetClasses(bool includeInner) const {
    auto &api = Internal::api;
    std::vector<IL2CPP::Il2CppClass *> classes{};

    if (_data->nameToClassHashTable != (decltype(_data->nameToClassHashTable)) - 0x424e4d) {
        if (api.il2cpp_image_get_class_count && api.il2cpp_image_get_class) {
            size_t typeCount = api.il2cpp_image_get_class_count(_data);
            for (size_t i = 0; i < typeCount; ++i) {
                auto cls = api.il2cpp_image_get_class(_data, i);
                if (!cls) continue;
                auto declaring = api.il2cpp_class_get_declaring_type(cls);
                if (!includeInner && declaring) continue;
                auto name = api.il2cpp_class_get_name(cls);
                if (!includeInner && (!api.il2cpp_class_get_flags(cls) || (name && strcmp(name, "<Module>") == 0))) continue;
                classes.push_back(cls);
            }
        }
    }

    ClassesManagement::ForEachClassInImage(_data, [&classes, includeInner](IL2CPP::Il2CppClass *cls) -> bool {
        if (!includeInner && Internal::api.il2cpp_class_get_declaring_type(cls)) return false;
        classes.push_back(cls);
        return false;
    });

    std::vector<BNM::Class> ret{};
    ret.reserve(classes.size());
    for (auto cls : classes) ret.emplace_back(cls);
    return ret;
}

std::vector<BNM::Image> Image::GetImages() {
    auto &assemblies = Internal::GetAllAssemblies();

    std::vector<Image> ret{};
    ret.reserve(assemblies.size());
    for (auto assembly : assemblies) {
        if (!assembly) continue;
        ret.emplace_back(Internal::api.il2cpp_assembly_get_image(assembly));
    }

    return ret;
}
