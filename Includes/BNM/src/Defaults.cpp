#include "BNM.hpp"

using namespace BNM;

namespace BNM::Defaults::Internal {
    BNM::Class Void{}, Boolean{}, Byte{}, SByte{}, Int16{}, UInt16{}, Int32{}, UInt32{}, IntPtr{}, UIntPtr{}, Int64{}, UInt64{}, Single{}, Double{}, Decimal{}, String{}, Object{};
    BNM::Class Type{};
    BNM::Class Vector2{}, Vector3{}, Vector4{}, Color{}, Color32{}, Ray{}, Quaternion{}, Matrix3x3{}, Matrix4x4{}, RaycastHit{}, RaycastHit2D{};
    BNM::Class UnityObject{}, MonoBehaviour{};
}

void BNM::Internal::LoadDefaults() {
    using namespace BNM::Defaults::Internal;
    auto &api = BNM::Internal::api;

    auto image = api.il2cpp_get_corlib ? api.il2cpp_get_corlib() : nullptr;
    auto SystemStr = "System";
    Void = TryGetClassInImage(image, SystemStr, "Void");
    Boolean = TryGetClassInImage(image, SystemStr, "Boolean");
    Byte = TryGetClassInImage(image, SystemStr, "Byte");
    SByte = TryGetClassInImage(image, SystemStr, "SByte");
    Int16 = TryGetClassInImage(image, SystemStr, "Int16");
    UInt16 = TryGetClassInImage(image, SystemStr, "UInt16");
    Int32 = TryGetClassInImage(image, SystemStr, "Int32");
    UInt32 = TryGetClassInImage(image, SystemStr, "UInt32");
    IntPtr = TryGetClassInImage(image, SystemStr, "IntPtr");
    UIntPtr = TryGetClassInImage(image, SystemStr, "UIntPtr");
    Int64 = TryGetClassInImage(image, SystemStr, "Int64");
    UInt64 = TryGetClassInImage(image, SystemStr, "UInt64");
    Single = TryGetClassInImage(image, SystemStr, "Single");
    Double = TryGetClassInImage(image, SystemStr, "Double");
    Decimal = TryGetClassInImage(image, SystemStr, "Decimal");
    String = TryGetClassInImage(image, SystemStr, "String");
    Object = TryGetClassInImage(image, SystemStr, "Object");
    Type = TryGetClassInImage(image, SystemStr, "Type");

    auto UnityEngineStr = "UnityEngine";
    image = TryGetImage("UnityEngine.CoreModule.dll");
    Vector2 = TryGetClassInImage(image, UnityEngineStr, "Vector2");
    Vector3 = TryGetClassInImage(image, UnityEngineStr, "Vector3");
    Vector4 = TryGetClassInImage(image, UnityEngineStr, "Vector4");
    Color = TryGetClassInImage(image, UnityEngineStr, "Color");
    Color32 = TryGetClassInImage(image, UnityEngineStr, "Color32");
    Ray = TryGetClassInImage(image, UnityEngineStr, "Ray");
    Quaternion = TryGetClassInImage(image, UnityEngineStr, "Quaternion");
    Matrix3x3 = TryGetClassInImage(image, UnityEngineStr, "Matrix3x3");
    Matrix4x4 = TryGetClassInImage(image, UnityEngineStr, "Matrix4x4");

    RaycastHit = TryGetClassInImage(TryGetImage("UnityEngine.PhysicsModule.dll"), UnityEngineStr, "RaycastHit");

    RaycastHit2D = TryGetClassInImage(TryGetImage("UnityEngine.Physics2DModule.dll"), UnityEngineStr, "RaycastHit2D");

    UnityObject = TryGetClassInImage(image, UnityEngineStr, "Object");
    MonoBehaviour = TryGetClassInImage(image, UnityEngineStr, "MonoBehaviour");
}
