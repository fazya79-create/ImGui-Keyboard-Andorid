#pragma once

#include "Debug.hpp"
#include "Il2CppStructures.hpp"
#include "Internals.hpp"
#include "Class.hpp"

namespace BNM {
namespace Structures {
namespace Unity {

struct Vector2;
struct Vector3;
struct Vector4;
struct Color;
struct Quaternion;
struct Vector3Int;

    struct Vector2;
    struct Vector3;

    struct Vector2 {
        union {
            struct { float x, y; };
            float data[2]{0.f, 0.f};
        };
        inline constexpr Vector2() : x(0), y(0) {};
        inline constexpr Vector2(float x, float y) : x(x), y(y) {};

        inline float* GetPtr() { return data; }
        [[nodiscard]] inline const float* GetPtr() const { return data; }

        inline float& operator[](int i) { return data[i]; }
        inline const float& operator[](int i) const { return data[i]; }

        inline static float Angle(Vector2 from, Vector2 to);
        inline static Vector2 ClampMagnitude(Vector2 vector, float maxLength);
        inline static float Component(Vector2 a, Vector2 b) { return Dot(a, b) / Magnitude(b); }
        inline static float Distance(Vector2 a, Vector2 b) { return Magnitude(a - b); }
        inline static float Dot(Vector2 lhs, Vector2 rhs) { return lhs.x * rhs.x + lhs.y * rhs.y; }
        inline static Vector2 FromPolar(float rad, float theta);
        inline static Vector2 Lerp(Vector2 from, Vector2 to, float t) { return to * t + from * (1.0f - t); }
        inline static Vector2 LerpUnclamped(Vector2 from, Vector2 to, float t) { return (to - from) * t + from; }
        inline static float Magnitude(Vector2 vector) { return sqrtf(vector.x * vector.x + vector.y * vector.y); }
        inline static Vector2 Max(Vector2 lhs, Vector2 rhs);
        inline static Vector2 Min(Vector2 lhs, Vector2 rhs);
        inline static Vector2 MoveTowards(Vector2 current, Vector2 target, float maxDistanceDelta);
        inline void Normalize() { *this = Normalize(*this); }
        inline static Vector2 Normalize(Vector2 vector);
        inline static void OrthoNormalize(Vector2 &normal, Vector2 &tangent);
        inline static Vector2 Perpendicular(Vector2 inDirection) { return {-inDirection.y, inDirection.x}; }
        inline static Vector2 Project(Vector2 a, Vector2 b);
        inline static Vector2 Reflect(Vector2 inDirection, Vector2 inNormal);
        inline static Vector2 Reject(Vector2 a, Vector2 b);
        inline static Vector2 RotateTowards(Vector2 current, Vector2 target, float maxRadiansDelta, float maxMagnitudeDelta);
        inline void Scale(Vector2 scale) { scale * scale; }
        inline static Vector2 Scale(Vector2 a, Vector2 b) { return a * b; }
        inline static Vector2 Slerp(Vector2, Vector2, float);
        inline static Vector2 SlerpUnclamped(Vector2, Vector2, float);
        inline static Vector2 SmoothDamp(Vector2 current, Vector2 target, Vector2 &currentVelocity, float smoothTime, float maxSpeed, float deltaTime);
        inline static float SqrMagnitude(Vector2 vector)  { return vector.x * vector.x + vector.y * vector.y; }
        inline static void ToPolar(Vector2, float &, float &);

        inline float magnitude() { return Magnitude(*this); }
        inline float sqrMagnitude() { return SqrMagnitude(*this); }
        inline Vector2 normalized() { return Normalize(*this); }
        inline std::string str() const { return std::to_string(x) + std::string(", ") + std::to_string(y); }

        inline bool operator==(const Vector2 v) { return x == v.x && y == v.y; }
        inline bool operator!=(const Vector2 v) { return x != v.x || y != v.y; }
        inline Vector2& operator+=(float rhs) { x += rhs; y += rhs; return *this; }
        inline Vector2& operator-=(float rhs) { x -= rhs; y -= rhs; return *this; }
        inline Vector2& operator*=(float rhs) { x *= rhs; y *= rhs; return *this; }
        inline Vector2& operator/=(float rhs) { x /= rhs; y /= rhs; return *this; }
        inline Vector2& operator+=(Vector2 rhs) { x += rhs.x; y += rhs.y; return *this; }
        inline Vector2& operator-=(Vector2 rhs) { x -= rhs.x; y -= rhs.y; return *this; }
        inline friend Vector2 operator+(Vector2 lhs, float rhs) { return {lhs.x + rhs, lhs.y + rhs}; }
        inline friend Vector2 operator-(Vector2 lhs, float rhs) { return {lhs.x - rhs, lhs.y - rhs}; }
        inline friend Vector2 operator*(Vector2 lhs, float rhs) { return {lhs.x * rhs, lhs.y * rhs}; }
        inline friend Vector2 operator/(Vector2 lhs, float rhs) { return {lhs.x / rhs, lhs.y / rhs}; }
        inline friend Vector2 operator+(float lhs, Vector2 rhs) { return {lhs + rhs.x, lhs + rhs.y}; }
        inline friend Vector2 operator-(float lhs, Vector2 rhs) { return {lhs - rhs.x, lhs - rhs.y}; }
        inline friend Vector2 operator*(float lhs, Vector2 rhs) { return {lhs * rhs.x, lhs * rhs.y}; }
        inline friend Vector2 operator/(float lhs, Vector2 rhs) { return {lhs / rhs.x, lhs / rhs.y}; }
        inline friend Vector2 operator+(Vector2 lhs, Vector2 rhs) { return {lhs.x + rhs.x, lhs.y + rhs.y}; }
        inline friend Vector2 operator-(Vector2 lhs, Vector2 rhs) { return {lhs.x - rhs.x, lhs.y - rhs.y}; }
        inline friend Vector2 operator*(Vector2 lhs, Vector2 rhs) { return {lhs.x * rhs.x, lhs.y * rhs.y}; }
        inline friend Vector2 operator/(Vector2 lhs, Vector2 rhs) { return {lhs.x / rhs.x, lhs.y / rhs.y}; }
        inline Vector2 operator-() const { return {-x, -y}; }
        operator Vector3() const;

        static const Vector2 positiveInfinity;
        static const Vector2 negativeInfinity;
        static const Vector2 down;
        static const Vector2 left;
        static const Vector2 one;
        static const Vector2 right;
        static const Vector2 up;
        static const Vector2 zero;
    };

    float Vector2::Angle(Vector2 a, Vector2 b) {
        float v = Dot(a, b) / (Magnitude(a) * Magnitude(b));
        v = fmaxf(v, -1.f);
        v = fminf(v, 1.f);
        return acos(v);
    }

    Vector2 Vector2::ClampMagnitude(Vector2 vector, float maxLength) {
        float length = Magnitude(vector);
        if (length > maxLength) vector *= maxLength / length;
        return vector;
    }

    Vector2 Vector2::FromPolar(float rad, float theta) {
        Vector2 v;
        v.x = rad * cosf(theta);
        v.y = rad * sinf(theta);
        return v;
    }

    Vector2 Vector2::Max(Vector2 a, Vector2 b) {
        float x = a.x > b.x ? a.x : b.x;
        float y = a.y > b.y ? a.y : b.y;
        return {x, y};
    }

    Vector2 Vector2::Min(Vector2 a, Vector2 b) {
        float x = a.x > b.x ? b.x : a.x;
        float y = a.y > b.y ? b.y : a.y;
        return {x, y};
    }

    Vector2 Vector2::MoveTowards(Vector2 current, Vector2 target, float maxDistanceDelta) {
        Vector2 d = target - current;
        float m = Magnitude(d);
        if (m < maxDistanceDelta || m == 0) return target;
        return current + (d * maxDistanceDelta / m);
    }

    Vector2 Vector2::Normalize(Vector2 vector) {
        float mag = Magnitude(vector);
        if (mag == 0) return Vector2::zero;
        return vector / mag;
    }

    void Vector2::OrthoNormalize(Vector2 &normal, Vector2 &tangent) {
        normal = Normalize(normal);
        tangent = Reject(tangent, normal);
        tangent = Normalize(tangent);
    }

    Vector2 Vector2::Project(Vector2 a, Vector2 b) {
        float m = Magnitude(b);
        return Dot(a, b) / (m * m) * b;
    }

    Vector2 Vector2::Reflect(Vector2 inDirection, Vector2 inNormal) {
        return inDirection - 2 * Project(inDirection, inNormal);
    }

    Vector2 Vector2::Reject(Vector2 a, Vector2 b) {
        return a - Project(a, b);
    }

    Vector2 Vector2::RotateTowards(Vector2 current, Vector2 target, float maxRadiansDelta, float maxMagnitudeDelta) {
        float magCur = Magnitude(current);
        float magTar = Magnitude(target);
        float newMag = magCur + maxMagnitudeDelta * (float) ((magTar > magCur) - (magCur > magTar));
        newMag = fminf(newMag, fmaxf(magCur, magTar));
        newMag = fmaxf(newMag, fminf(magCur, magTar));
        float totalAngle = Angle(current, target) - maxRadiansDelta;
        if (totalAngle <= 0) return Normalize(target) * newMag;
        else if (totalAngle >= M_PI) return -Normalize(target) * newMag;
        float axis = current.x * target.y - current.y * target.x;
        axis = axis / fabsf(axis);
        if (1 - fabsf(axis) >= 0.00001) axis = 1;
        current = Normalize(current);
        Vector2 newVector = current * cosf(maxRadiansDelta) + Vector2(-current.y, current.x) * sinf(maxRadiansDelta) * axis;
        return newVector * newMag;
    }

    Vector2 Vector2::Slerp(Vector2 a, Vector2 b, float t) {
        if (t < 0) return a;
        else if (t > 1) return b;
        return SlerpUnclamped(a, b, t);
    }

    Vector2 Vector2::SlerpUnclamped(Vector2 a, Vector2 b, float t) {
        float magA = Magnitude(a);
        float magB = Magnitude(b);
        a /= magA;
        b /= magB;
        float dot = Dot(a, b);
        dot = fmaxf(dot, -1.f);
        dot = fminf(dot, 1.f);
        float theta = acosf(dot) * t;
        Vector2 relativeVec = Normalize(b - a * dot);
        Vector2 newVec = a * cosf(theta) + relativeVec * sinf(theta);
        return newVec * (magA + (magB - magA) * t);
    }

    Vector2 Vector2::SmoothDamp(Vector2 current, Vector2 target, Vector2 &currentVelocity, float smoothTime, float maxSpeed, float deltaTime) {
        smoothTime = std::max(0.0001f, smoothTime);
        float omega = 2.f / smoothTime;
        float x = omega * deltaTime;
        float exp = 1.f / (1.f + x + 0.48f * x * x + 0.235f * x * x * x);
        Vector2 change = current - target;
        Vector2 originalTo = target;
        float maxChange = maxSpeed * smoothTime;
        float maxChangeSq = maxChange * maxChange;
        float sqrMag = SqrMagnitude(change);
        if (sqrMag > maxChangeSq) change /= sqrtf(sqrMag) * maxChange;
        target = current - change;
        Vector2 temp = (currentVelocity + omega * change) * deltaTime;
        currentVelocity = (currentVelocity - omega * temp) * exp;
        Vector2 output = target + (change + temp) * exp;
        Vector2 origMinusCurrent = originalTo - current;
        Vector2 outMinusOrig = output - originalTo;
        if (origMinusCurrent.x * outMinusOrig.x + origMinusCurrent.y * outMinusOrig.y > 0.f) {
            currentVelocity = (output - originalTo) / deltaTime;
            return originalTo;
        }
        return output;
    }

    void Vector2::ToPolar(Vector2 vector, float &rad, float &theta) {
        rad = Magnitude(vector);
        theta = atan2f(vector.y, vector.x);
    }

    struct Vector2Int {
        union {
            struct { int x, y; };
            int data[2]{0, 0};
        };
    };

    struct Quaternion;

    struct Vector3 {
        union {
            struct { float x, y, z; };
            float data[3]{0.f, 0.f, 0.f};
        };
        inline constexpr Vector3() = default;
        inline constexpr Vector3(float x, float y, float z) : x(x), y(y), z(z) {};

        operator Vector4() const;
        inline operator Vector2&() { return *(Vector2 *) this; }
        inline operator Vector2() const { return {x, y}; }

        inline float* GetPtr() { return data; }
        [[nodiscard]] inline const float* GetPtr() const { return data; }

        inline float& operator[](int i) { return data[i]; }
        inline const float& operator[](int i) const { return data[i]; }

        inline static float Angle(Vector3 from, Vector3 to);
        inline static float AngleBetween(Vector3 from, Vector3 to);
        inline static Vector3 ClampMagnitude(Vector3 vector, float maxLength);
        inline static Vector3 Cross(Vector3 lhs, Vector3 rhs) { return {lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x}; }
        inline static float Component(Vector3 a, Vector3 b) { return Dot(a, b) / Magnitude(b); }
        inline static float Distance(Vector3 a, Vector3 b) { return Magnitude(a - b); }
        inline static float Dot(Vector3 lhs, Vector3 rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; };
        inline static Vector3 FromSpherical(float rad, float theta, float phi);
        inline static Vector3 FromString(const std::string &str);
        inline static Vector3 Lerp(Vector3 from, Vector3 to, float t) { return to * t + from * (1.0f - t); }
        inline static Vector3 LerpUnclamped(Vector3 from, Vector3 to, float t) { return (to - from) * t + from; }
        inline static float Magnitude(Vector3 vector) { return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z); }
        inline static Vector3 Max(Vector3 lhs, Vector3 rhs);
        inline static Vector3 Min(Vector3 lhs, Vector3 rhs);
        inline static Vector3 MoveTowards(Vector3 current, Vector3 target, float maxDistanceDelta);
        inline void Normalize() { *this = Normalize(*this); }
        inline static float NormalizeAngle(float f, bool is180 = true);
        inline static Vector3 NormalizeEuler(Vector3 vector, bool is180 = true);
        inline static Vector3 Normalize(Vector3 value);
        inline static Vector3 Orthogonal(Vector3 vector);
        inline static void OrthoNormalize(Vector3 &normal, Vector3 &tangent);
        inline static void OrthoNormalize(Vector3 &normal, Vector3 &tangent, Vector3 &binormal);
        inline static void OrthoNormalizeFast(Vector3 &normal, Vector3 &tangent, Vector3 &binormal);
        inline static Vector3 OrthoNormalVectorFast(const Vector3& n);
        inline static Vector3 Project(Vector3 a, Vector3 b);
        inline static Vector3 ProjectOnPlane(Vector3 vector, Vector3 planeNormal);
        inline static Vector3 Reflect(Vector3 inDirection, Vector3 inNormal);
        inline static Vector3 RotateTowards(Vector3 current, Vector3 target, float maxRadiansDelta, float maxMagnitudeDelta);
        inline void Scale(Vector3 scale) { *this = *this * scale; }
        inline static Vector3 Scale(Vector3 a, Vector3 b) { return a * b; }
        inline static float SignedAngle(Vector3 from, Vector3 to, Vector3 axis);
        inline static Vector3 Slerp(Vector3 a, Vector3 b, float t);
        inline static Vector3 SlerpUnclamped(Vector3 a, Vector3 b, float t);
        inline static Vector3 SmoothDamp(Vector3 current, Vector3 target, Vector3 &currentVelocity, float smoothTime, float maxSpeed, float deltaTime);
        inline static float SqrMagnitude(Vector3 vector) { return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z; }
        inline static void ToSpherical(Vector3 vector, float &rad, float &theta, float &phi);

        inline std::string str() { return std::to_string(x) + std::string(", ") + std::to_string(y) + std::string(", ") + std::to_string(z); }
        inline Vector3 orthogonal() const { return Orthogonal(*this); }
        inline float magnitude() const { return Magnitude(*this); }
        inline Vector3 normalized() const { return Normalize(*this); }
        inline float sqrMagnitude() const { return SqrMagnitude(*this); }
        inline Vector3 normalizedEuler(bool is180 = true) const { return NormalizeEuler(*this, is180); }

        inline bool operator==(Vector3 v) const { return x == v.x && y == v.y && z == v.z; }
        inline bool operator!=(Vector3 v) const { return x != v.x || y != v.y || z != v.z; }
        inline Vector3& operator+=(Vector3 inV) { x += inV.x; y += inV.y; z += inV.z; return *this; }
        inline Vector3& operator-=(Vector3 inV) { x -= inV.x; y -= inV.y; z -= inV.z; return *this; }
        inline Vector3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
        inline Vector3& operator/=(float s)  { x /= s; y /= s; z /= s; return *this; }
        inline Vector3& operator*=(int s) { x *= s; y *= s; z *= s; return *this; }
        inline Vector3& operator/=(int s)  { x /= s; y /= s; z /= s; return *this; }
        inline Vector3& operator/=(Vector3 inV) { x /= inV.x; y /= inV.y; z /= inV.z; return *this; }
        inline friend Vector3 operator+(Vector3 lhs, Vector3 rhs) { return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z}; }
        inline friend Vector3 operator-(Vector3 lhs, Vector3 rhs) { return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z}; }
        inline friend Vector3 operator*(Vector3 v, float s) { return {v.x * s, v.y * s, v.z * s}; }
        inline friend Vector3 operator*(Vector3 v, int s) { return {v.x * s, v.y * s, v.z * s}; }
        inline friend Vector3 operator*(float s, Vector3 v) { return {v.x * s, v.y * s, v.z * s}; }
        inline friend Vector3 operator*(int s, Vector3 v) { return {v.x * s, v.y * s, v.z * s}; }
        inline friend Vector3 operator*(Vector3 lhs, Vector3 rhs) { return {lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z}; }
        inline friend Vector3 operator/(Vector3 v, float s) { Vector3 temp(v); temp /= s; return temp; }
        inline friend Vector3 operator/(Vector3 v, int s) { Vector3 temp(v); temp /= s; return temp; }
        inline friend Vector3 operator/(Vector3 lhs, Vector3 rhs) { return {lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z}; }
        inline Vector3 operator-() const { return {-x, -y, -z}; }

        inline static constexpr const float kEpsilon = 1E-05f;
        inline static constexpr const float kEpsilonNormalSqrt = 1E-15f;
        static const Vector3 positiveInfinity;
        static const Vector3 negativeInfinity;
        static const Vector3 back;
        static const Vector3 down;
        static const Vector3 forward;
        static const Vector3 left;
        static const Vector3 one;
        static const Vector3 right;
        static const Vector3 up;
        static const Vector3 zero;
    };

    float Vector3::Angle(Vector3 from, Vector3 to) {
        float v = Dot(from, to) / (Magnitude(from) * Magnitude(to));
        v = fmaxf(v, -1.f);
        v = fminf(v, 1.f);
        return acosf(v);
    }

    float Vector3::AngleBetween(Vector3 from, Vector3 to) {
        float v = Dot(Normalize(from), Normalize(to));
        v = fmaxf(v, -1.f);
        v = fminf(v, 1.f);
        return acosf(v);
    }

    Vector3 Vector3::ClampMagnitude(Vector3 vector, float maxLength) {
        float length = Magnitude(vector);
        if (length > maxLength) vector *= maxLength / length;
        return vector;
    }

    Vector3 Vector3::FromSpherical(float rad, float theta, float phi) {
        Vector3 v{};
        v.x = rad * sinf(theta) * cosf(phi);
        v.y = rad * sinf(theta) * sinf(phi);
        v.z = rad * cosf(theta);
        return v;
    }

    Vector3 Vector3::FromString(const std::string& str) {
        std::vector<std::string> args{};
        std::string buffer{};
        for (char i : str) {
            bool no = true;
            if (i == 'f') no = false;
            if (i != ',') buffer += i;
            else if (no) {
                args.push_back(buffer);
                buffer.clear();
            }
        }
        if (!buffer.empty()) args.push_back(buffer);
        if (args.size() != 3) return {};
        return {std::stof(args[0]), std::stof(args[1]), std::stof(args[2])};
    }

    Vector3 Vector3::Max(Vector3 a, Vector3 b) {
        float x = a.x > b.x ? a.x : b.x;
        float y = a.y > b.y ? a.y : b.y;
        float z = a.z > b.z ? a.z : b.z;
        return {x, y, z};
    }

    Vector3 Vector3::Min(Vector3 a, Vector3 b) {
        float x = a.x > b.x ? b.x : a.x;
        float y = a.y > b.y ? b.y : a.y;
        float z = a.z > b.z ? b.z : a.z;
        return {x, y, z};
    }

    Vector3 Vector3::MoveTowards(Vector3 current, Vector3 target, float maxDistanceDelta) {
        Vector3 d = target - current;
        float m = Magnitude(d);
        if (m < maxDistanceDelta || m == 0)
            return target;
        return current + (d * maxDistanceDelta / m);
    }

    Vector3 Vector3::Normalize(Vector3 v) {
        float mag = Magnitude(v);
        if (mag == 0) return Vector3::zero;
        return v / mag;
    }

    float Vector3::NormalizeAngle(float f, bool is180) {
        while (f > (is180 ? 180.f : 360.f)) f -= 360.f;
        while (f < (is180 ? -180.f : 0.f)) f += 360.f;
        return f;
    }

    Vector3 Vector3::NormalizeEuler(Vector3 vec, bool is180) {
        vec.x = NormalizeAngle(vec.x, is180);
        vec.y = NormalizeAngle(vec.y, is180);
        vec.z = NormalizeAngle(vec.z, is180);
        return vec;
    }

    Vector3 Vector3::Orthogonal(Vector3 vector) {
        return vector.z < vector.x ? Vector3(vector.y, -vector.x, 0) : Vector3(0, -vector.z, vector.y);
    }

    void Vector3::OrthoNormalize(Vector3 &inU, Vector3 &inV) {
        float mag = Magnitude(inU);
        if (mag > 0.00001) inU /= mag;
        else inU = {1, 0, 0};
        float dot0 = Dot(inU, inV);
        inV -= dot0 * inU;
        mag = Magnitude(inV);
        if (mag < 0.00001) inV = OrthoNormalVectorFast(inU);
        else inV /= mag;
    }

    void Vector3::OrthoNormalize(Vector3 &inU, Vector3 &inV, Vector3 &inW) {
        float mag = Magnitude(inU);
        if (mag > kEpsilon) inU /= mag;
        else inU = {1, 0, 0};
        float dot0 = Dot(inU, inV);
        inV -= dot0 * inU;
        mag = Magnitude(inV);
        if (mag > kEpsilon) inV /= mag;
        else inV = OrthoNormalVectorFast(inU);
        float dot1 = Dot(inV, inW);
        dot0 = Dot(inU, inW);
        inW -= dot0 * inU + dot1 * inV;
        mag = Magnitude(inW);
        if (mag > kEpsilon) inW /= mag;
        else inW = Cross(inU, inV);
    }

    void Vector3::OrthoNormalizeFast(Vector3 &inU, Vector3 &inV, Vector3 &inW) {
        inU = Normalize(inU);
        float dot0 = Dot(inU, inV);
        inV -= dot0 * inU;
        inV = Normalize(inV);
        float dot1 = Dot(inV, inW);
        dot0 = Dot(inU, inW);
        inW -= dot0 * inU + dot1 * inV;
        inW = Normalize(inW);
    }

    Vector3 Vector3::OrthoNormalVectorFast(const Vector3& n) {
        Vector3 res{};
        constexpr float k1OverSqrt2 = 0.7071067811865475244008443621048490f;
        if (abs(n.z) > k1OverSqrt2) {
            float a = n.y * n.y + n.z * n.z;
            float k = 1 / sqrt(a);
            res.x = 0;
            res.y = -n.z * k;
            res.z = n.y * k;
        } else {
            float a = n.x * n.x + n.y * n.y;
            float k = 1 / sqrt(a);
            res.x = -n.y * k;
            res.y = n.x * k;
            res.z = 0;
        }
        return res;
    }

    Vector3 Vector3::Project(Vector3 vector, Vector3 onNormal) {
        float sqrMag = Dot(onNormal, onNormal);
        if (sqrMag < kEpsilon) return Vector3::zero;
        float dot = Dot(vector, onNormal);
        return onNormal * dot / sqrMag;
    }

    Vector3 Vector3::ProjectOnPlane(Vector3 vector, Vector3 planeNormal) {
        float sqrMag = Dot(planeNormal, planeNormal);
        if (sqrMag < kEpsilon) return vector;
        float dot = Dot(vector, planeNormal);
        return vector - planeNormal * dot / sqrMag;
    }

    Vector3 Vector3::Reflect(Vector3 inDirection, Vector3 inNormal) {
        return (-2.f * Dot(inNormal, inDirection)) * inNormal + inDirection;
    }

    Vector3 Vector3::RotateTowards(Vector3 current, Vector3 target, float maxRadiansDelta, float maxMagnitudeDelta) {
        float magCur = Magnitude(current);
        float magTar = Magnitude(target);
        float newMag = magCur + maxMagnitudeDelta * (float) ((magTar > magCur) - (magCur > magTar));
        newMag = fminf(newMag, fmaxf(magCur, magTar));
        newMag = fmaxf(newMag, fminf(magCur, magTar));
        float totalAngle = Angle(current, target) - maxRadiansDelta;
        if (totalAngle <= 0) return Normalize(target) * newMag;
        else if (totalAngle >= M_PI) return -Normalize(target) * newMag;
        Vector3 axis = Cross(current, target);
        float magAxis = Magnitude(axis);
        if (magAxis == 0) axis = Normalize(Cross(current, current + Vector3(3.95, 5.32, -4.24)));
        else axis /= magAxis;
        current = Normalize(current);
        Vector3 newVector = current * cosf(maxRadiansDelta) + Cross(axis, current) * sinf(maxRadiansDelta);
        return newVector * newMag;
    }

    float Vector3::SignedAngle(Vector3 from, Vector3 to, Vector3 axis) {
        float unsignedAngle = Angle(from, to);
        float cross_x = from.y * to.z - from.z * to.y;
        float cross_y = from.z * to.x - from.x * to.z;
        float cross_z = from.x * to.y - from.y * to.x;
        float sign = (axis.x * cross_x + axis.y * cross_y + axis.z * cross_z) >= 0.f ? 1.f : -1.f;
        return unsignedAngle * sign;
    }

    Vector3 Vector3::Slerp(Vector3 a, Vector3 b, float t) {
        if (t < 0) return a;
        else if (t > 1) return b;
        return SlerpUnclamped(a, b, t);
    }

    Vector3 Vector3::SlerpUnclamped(Vector3 a, Vector3 b, float t) {
        float magA = Magnitude(a);
        float magB = Magnitude(b);
        a /= magA;
        b /= magB;
        float dot = Dot(a, b);
        dot = fmaxf(dot, -1.f);
        dot = fminf(dot, 1.f);
        float theta = acosf(dot) * t;
        Vector3 relativeVec = b - a * dot;
        Vector3 newVec = a * cosf(theta) + Normalize(relativeVec) * sinf(theta);
        return newVec * (magA + (magB - magA) * t);
    }

    Vector3 Vector3::SmoothDamp(Vector3 current, Vector3 target, Vector3 &currentVelocity, float smoothTime, float maxSpeed, float deltaTime) {
        smoothTime = std::max(0.0001f, smoothTime);
        float omega = 2.f / smoothTime;
        float x = omega * deltaTime;
        float exp = 1.f / (1.f + x + 0.48f * x * x + 0.235f * x * x * x);
        Vector3 change = current - target;
        Vector3 originalTo = target;
        float maxChange = maxSpeed * smoothTime;
        float maxChangeSq = maxChange * maxChange;
        float sqrMag = SqrMagnitude(change);
        if (sqrMag > maxChangeSq) change /= sqrtf(sqrMag) * maxChange;
        target = current - change;
        Vector3 temp = (currentVelocity + omega * change) * deltaTime;
        currentVelocity = (currentVelocity - omega * temp) * exp;
        Vector3 output = target + (change + temp) * exp;
        Vector3 origMinusCurrent = originalTo - current;
        Vector3 outMinusOrig = output - originalTo;
        if (origMinusCurrent.x * outMinusOrig.x + origMinusCurrent.y * outMinusOrig.y + origMinusCurrent.z * outMinusOrig.z > 0.f) {
            currentVelocity = (output - originalTo) / deltaTime;
            return originalTo;
        }
        return output;
    }

    void Vector3::ToSpherical(Vector3 vector, float &rad, float &theta, float &phi) {
        rad = Magnitude(vector);
        float v = vector.z / rad;
        v = fmaxf(v, -1.f);
        v = fminf(v, 1.f);
        theta = acosf(v);
        phi = atan2f(vector.y, vector.x);
    }

    struct Vector3Int {
        union {
            struct { int x, y, z; };
            int data[3]{0, 0, 0};
        };
    };

    struct Vector4;

    inline bool IsFinite(float value) { return (*(uint32_t *) &value & 0x7f800000) != 0x7f800000; }

    struct Vector4 {
        union {
            struct { float x, y, z, w; };
            float data[4]{0.f, 0.f, 0.f, 0.f};
        };
        inline constexpr Vector4() = default;
        inline constexpr Vector4(float inX, float inY, float inZ, float inW) : x(inX), y(inY), z(inZ), w(inW) {}
        inline constexpr Vector4(Vector3 v, float inW) : x(v.x), y(v.y), z(v.z), w(inW) {}
        inline Vector4(Color c);

        inline float* GetPtr() { return data; }
        [[nodiscard]] inline const float* GetPtr() const { return data; }

        inline float& operator[](int i) { return data[i]; }
        inline const float& operator[](int i) const { return data[i]; }

        inline static float Component(Vector4 a, Vector4 b) { return Dot(a, b) / Magnitude(b); }
        inline static float Distance(Vector4 a, Vector4 b) { return Magnitude(a - b); }
        inline static float Dot(Vector4 lhs, Vector4 rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w; }
        inline static bool IsFinite(Vector4 f) { return Unity::IsFinite(f.x) && Unity::IsFinite(f.y) && Unity::IsFinite(f.z) && Unity::IsFinite(f.w); }
        inline static Vector4 Lerp(Vector4 from, Vector4 to, float t) { return to * t + from * (1.0f - t); }
        inline static Vector4 LerpUnclamped(Vector4 from, Vector4 to, float t) { return (to - from) * t + from; }
        inline static float Magnitude(Vector4 inV) { return sqrt(Dot(inV, inV)); }
        inline void Normalize() { *this = Normalize(*this); }
        inline static Vector4 Normalize(Vector4 v) { float n = Magnitude(v); if (n > 1E-05f) return v / n; return zero; }
        inline static Vector4 Project(Vector4 a, Vector4 b) { return b * (Dot(a, b) / Dot(b, b)); }
        inline static bool CompareApproximately(Vector4 a, Vector4 b, float inMaxDist = Vector3::kEpsilon) { return SqrMagnitude(b - a) <= inMaxDist * inMaxDist; }
        inline static float SqrMagnitude(Vector4 inV) { return Dot(inV, inV); }

        bool operator==(Vector4 v) const { return x == v.x && y == v.y && z == v.z && w == v.w; }
        bool operator!=(Vector4 v) const { return x != v.x || y != v.y || z != v.z || w != v.w; }
        inline friend Vector4 operator*(Vector4 lhs, Vector4 rhs) { return {lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w}; }
        inline friend Vector4 operator*(Vector4 v, float s) { return {v.x * s, v.y * s, v.z * s, v.w * s}; }
        inline friend Vector4 operator+(Vector4 lhs, Vector4 rhs) { return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w}; }
        inline friend Vector4 operator-(Vector4 lhs, Vector4 rhs) { return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w}; }
        inline friend Vector4 operator/(Vector4 v, float s) { return {v.x / s, v.y / s, v.z / s, v.w / s}; }
        inline friend Vector4 operator/(Vector4 lhs, Vector4 rhs) { return {lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w}; }
        Vector4 operator-() const { return {-x, -y, -z, -w}; }
        inline operator Vector3() const { return {x, y, z}; }

        static const Vector4 positiveInfinity;
        static const Vector4 negativeInfinity;
        static const Vector4 zero;
        static const Vector4 one;
    };

    struct Vector4;

    struct Color {
        union {
            struct { float r, g, b, a; };
            float data[4]{0.f, 0.f, 0.f, 1.f};
        };
        inline constexpr Color() = default;
        inline constexpr Color(float r, float g, float b) : r(r), g(g), b(b), a(1.f) {}
        inline constexpr Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
        inline Color(Vector4 v);

        [[nodiscard]] inline std::string str() const { return std::string("r: ") + std::to_string(r) + std::string(", g: ") + std::to_string(g) + std::string(", b: ") + std::to_string(b) + std::string(", a: ") + std::to_string(a); };

        inline static Color HSVToRGB(float H, float S, float V, bool hdr = true) {
            if (S == 0) return {V, V, V};
            else if (V == 0) return {0.f, 0.f, 0.f};

            float h_to_floor = H * 6.0f;

            int tmp = (int) floor(h_to_floor);
            float F = h_to_floor - (float) tmp;

            float P = (V) * (1 - S);
            float Q = V * (1 - S * F);
            float T = V * (1 - S * (1 - F));

            Color retVal = {0.f, 0.f, 0.f};
            switch (tmp) {
                case 0: retVal = {V, T, P}; break;
                case 1: retVal = {Q, V, P}; break;
                case 2: retVal = {P, V, T}; break;
                case 3: retVal = {P, Q, V}; break;
                case 4: retVal = {T, P, V}; break;
                case 5: retVal = {V, P, Q}; break;
                case 6: retVal = {V, T, P}; break;
                case -1: retVal = {V, P, Q}; break;
            }
            if (hdr) return retVal;

            return {std::clamp(retVal.r, 0.0f, 1.0f), std::clamp(retVal.g, 0.0f, 1.0f), std::clamp(retVal.b, 0.0f, 1.0f)};
        }

        inline static Color Lerp(Color a, Color b, float t) {
            t = t < 0.f ? 0.f : t > 1.f ? 1.f : t;
            return {
                a.r + (b.r - a.r) * t,
                a.g + (b.g - a.g) * t,
                a.b + (b.b - a.b) * t,
                a.a + (b.a - a.a) * t
            };
        }

        inline Color RGBMultiplied(float m) { return {r * m, g * m, b * m, a}; }
        inline Color RGBMultiplied(Color m) { return {r * m.r, g * m.g, b * m.b, a}; }

        inline friend bool operator ==(const Color& lhs, const Color& rhs) { return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a; }
        inline friend bool operator !=(const Color& lhs, const Color& rhs) { return !(lhs == rhs); }

        static const Color black, red, green, blue, white, orange, yellow, cyan, magenta;
    };

    struct Color32 {
        union {
            struct { uint8_t r{}, g{}, b{}, a{}; };
            int rgba;
        };
    };

    struct Quaternion {
        union {
            struct { float x, y, z, w; };
            float data[4]{0.f, 0.f, 0.f, 0.f};
        };

        inline Quaternion() : x(0), y(0), z(0), w(1) {};
        inline Quaternion(float data[]) noexcept : x(data[0]), y(data[1]), z(data[2]), w(data[3]) {}
        inline Quaternion(Vector3 vector, float scalar) noexcept : x(vector.x), y(vector.y), z(vector.z), w(scalar) {};
        inline Quaternion(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {};
        inline Quaternion(float Yaw, float Pitch, float Roll) {
        };

        inline static Vector3 Up(Quaternion q);
        inline static Vector3 Down(Quaternion q);
        inline static Vector3 Left(Quaternion q);
        inline static Vector3 Right(Quaternion q);
        inline static Vector3 Forward(Quaternion q);
        inline static Vector3 Back(Quaternion q);

        inline static float Angle(Quaternion a, Quaternion b);
        inline static Quaternion Conjugate(Quaternion rotation);
        inline static float Dot(Quaternion lhs, Quaternion rhs);
        inline static Quaternion FromAngleAxis(float angle, Vector3 axis);
        inline static Quaternion FromEuler(Vector3 rotation);
        inline static Quaternion FromEuler(float yaw, float pitch, float roll, bool fromDeg = true);
        inline static Quaternion FromToRotation(Vector3 fromVector, Vector3 toVector);
        inline static Quaternion Inverse(Quaternion rotation);
        inline static Quaternion Lerp(Quaternion a, Quaternion b, float t);
        inline static Quaternion LerpUnclamped(Quaternion a, Quaternion b, float t);
        inline static Quaternion LookRotation(Vector3 forward);
        inline static Quaternion LookRotation(Vector3 forward, Vector3 upwards);
        inline static float Norm(Quaternion rotation);
        inline static Quaternion Normalize(Quaternion rotation);
        inline static Quaternion RotateTowards(Quaternion from, Quaternion to, float maxRadiansDelta);
        inline static Quaternion Slerp(Quaternion a, Quaternion b, float t);
        inline static Quaternion SlerpUnclamped(Quaternion a, Quaternion b, float t);
        inline static void ToAngleAxis(Quaternion rotation, float &angle, Vector3 &axis);
        inline static Vector3 ToEuler(Quaternion q, bool toDeg = true);

        inline Vector3 eulerAngles() { return ToEuler(*this); }
        inline Quaternion normalized() { return Normalize(*this); }

        inline std::string str() const {return std::to_string(x) + std::string(", ") + std::to_string(y) + std::string(", ") + std::to_string(z) + std::string(", ") + std::to_string(w); }

        inline Quaternion& operator+=(Quaternion q) { x += q.x; y += q.y; z += q.z; w += q.w; return *this; }
        inline Quaternion& operator-=(Quaternion q) { x -= q.x; y -= q.y; z -= q.z; w -= q.w; return *this; }
        inline Quaternion& operator*=(Quaternion rhs);
        inline Quaternion& operator*=(float s) { x *= s; y *= s; z *= s; w *= s; return *this; }
        inline Quaternion& operator/=(Quaternion rhs);
        inline Quaternion& operator/=(float s) { x /= s; y /= s; z /= s; w /= s; return *this; }
        inline friend Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs) { Quaternion q(lhs); return q += rhs; }
        inline friend Quaternion operator-(const Quaternion& lhs, const Quaternion& rhs) { Quaternion t(lhs); return t -= rhs; }
        inline friend Quaternion operator*(const float s, const Quaternion& q) { Quaternion t(q); return t *= s; }
        inline friend Quaternion operator/(const Quaternion& q, const float s) { Quaternion t(q); return t /= s; }
        inline Quaternion operator-() const { return {-x, -y, -z, -w}; }
        inline Quaternion operator*(float s) const { return {x * s, y * s, z * s, w * s}; }

        inline friend Quaternion operator*(Quaternion lhs, Quaternion rhs) {
            return {lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
                    lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z,
                    lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x,
                    lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z};
        }
        inline friend Quaternion operator/(Quaternion lhs, Quaternion rhs) {
            return {lhs.w / rhs.x + lhs.x / rhs.w + lhs.y / rhs.z - lhs.z / rhs.y,
                    lhs.w / rhs.y + lhs.y / rhs.w + lhs.z / rhs.x - lhs.x / rhs.z,
                    lhs.w / rhs.z + lhs.z / rhs.w + lhs.x / rhs.y - lhs.y / rhs.x,
                    lhs.w / rhs.w - lhs.x / rhs.x - lhs.y / rhs.y - lhs.z / rhs.z};
        }
        inline static Vector3 RotateVectorByQuaternion(Quaternion lhs, Vector3 rhs);

        static const Quaternion identity;
    };

    inline Quaternion& Quaternion::operator*=(Quaternion rhs) {
        float tempX = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
        float tempY = w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z;
        float tempZ = w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x;
        float tempW = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
        x = tempX; y = tempY; z = tempZ; w = tempW;
        return *this;
    }

    inline Quaternion& Quaternion::operator/=(Quaternion rhs) {
        float tempX = w / rhs.x + x / rhs.w + y / rhs.z - z / rhs.y;
        float tempY = w / rhs.y + y / rhs.w + z / rhs.x - x / rhs.z;
        float tempZ = w / rhs.z + z / rhs.w + x / rhs.y - y / rhs.x;
        float tempW = w / rhs.w - x / rhs.x - y / rhs.y - z / rhs.z;
        x = tempX; y = tempY; z = tempZ; w = tempW;
        return *this;
    }

    inline Vector3 operator*(Quaternion lhs, const Vector3 rhs) {
        Vector3 u = Vector3(lhs.x, lhs.y, lhs.z);
        return u * (Vector3::Dot(u, rhs) * 2.0f) + rhs * (lhs.w * lhs.w - Vector3::Dot(u, u)) + Vector3::Cross(u, rhs) * (2.0f * lhs.w);
    }

    Vector3 Quaternion::Up(Quaternion q) { return q * Vector3::up; }
    Vector3 Quaternion::Down(Quaternion q) { return q * Vector3::down; }
    Vector3 Quaternion::Left(Quaternion q) { return q * Vector3::left; }
    Vector3 Quaternion::Right(Quaternion q) { return q * Vector3::right; }
    Vector3 Quaternion::Forward(Quaternion q) { return q * Vector3::forward; }
    Vector3 Quaternion::Back(Quaternion q) { return q * Vector3::back; }

    float Quaternion::Angle(Quaternion a, Quaternion b) {
        float dot = Dot(a, b);
        return acosf(fminf(fabs(dot), 1)) * 2;
    }

    Quaternion Quaternion::Conjugate(Quaternion rotation) {
        return {-rotation.x, -rotation.y, -rotation.z, rotation.w};
    }

    float Quaternion::Dot(Quaternion lhs, Quaternion rhs) {
        return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
    }

    Quaternion Quaternion::FromAngleAxis(float angle, Vector3 axis) {
        Quaternion q;
        float m = sqrtf(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
        float s = sinf(angle / 2) / m;
        q.x = axis.x * s;
        q.y = axis.y * s;
        q.z = axis.z * s;
        q.w = cosf(angle / 2);
        return q;
    }

    Quaternion Quaternion::FromEuler(Vector3 rotation) {
        return FromEuler(rotation.x, rotation.y, rotation.z);
    }

    Quaternion Quaternion::FromEuler(float yaw, float pitch, float roll, bool fromDeg) {
        constexpr float deg2Rad = M_PI / 180.f;

        if (fromDeg) {
            roll *= (float) deg2Rad;
            pitch *= (float) deg2Rad;
            yaw *= (float) deg2Rad;
        }
        float cY(cosf(yaw / 2.0f));
        float sY(sinf(yaw / 2.0f));

        float cP(cosf(pitch / 2.0f));
        float sP(sinf(pitch / 2.0f));

        float cR(cosf(roll / 2.0f));
        float sR(sinf(roll / 2.0f));
        return {cP * sY * cR + sP * cY * sR,
                sP * cY * cR - cP * sY * sR,
                cP * cY * sR - sP * sY * cR,
                cP * cY * cR + sP * sY * sR};
    }

    Vector3 Quaternion::ToEuler(Quaternion q, bool toDeg) {
        Vector3 rot{};

        float xy = q.x * q.y, xw = q.x * q.w;
        float yz = q.y * q.z;
        float zw = q.z * q.w;

        float singularity_test = yz - xw;

        rot.x = -1.f * asinf(std::clamp(2.0f * singularity_test, -1.0f, 1.0f));

        if (abs(singularity_test) < 0.499999f) {
            float xx = q.x * q.x, yy = q.y * q.y, yw = q.y * q.w, zz = q.z * q.z, ww = q.w * q.w;
            rot.y = atan2f(2.0f * (q.x * q.z + yw), zz - xx - yy + ww);
            rot.z = atan2f(2.0f * (xy + zw), yy - zz - xx + ww);
        } else {
            float a = xy + zw;
            float b = -yz + xw;
            float c = xy - zw;
            float e = yz + xw;
            rot.y = atan2f(a * e + b * c, b * e - a * c);
            rot.z = 0;
        }

        constexpr float rad2deg = 180.f / M_PI;
        if (toDeg) rot *= (float) rad2deg;

        return rot;
    }
    Quaternion Quaternion::FromToRotation(Vector3 fromVector, Vector3 toVector) {
        float dot = Vector3::Dot(fromVector, toVector);
        float k = sqrtf(Vector3::SqrMagnitude(fromVector) * Vector3::SqrMagnitude(toVector));
        if (fabsf(dot / k + 1) < 0.00001) {
            Vector3 ortho = Vector3::Orthogonal(fromVector);
            return {Vector3::Normalize(ortho), 0};
        }
        Vector3 cross = Vector3::Cross(fromVector, toVector);
        return Normalize(Quaternion(cross, dot + k));
    }

    Quaternion Quaternion::Inverse(Quaternion rotation) {
        float n = Norm(rotation);
        return Conjugate(rotation) / (n * n);
    }

    Quaternion Quaternion::Lerp(Quaternion a, Quaternion b, float t) {
        if (t < 0) return Normalize(a);
        else if (t > 1) return Normalize(b);
        return LerpUnclamped(a, b, t);
    }

    Quaternion Quaternion::LerpUnclamped(Quaternion a, Quaternion b, float t) {
        Quaternion quaternion{};
        if (Dot(a, b) >= 0) quaternion = a * (1 - t) + b * t;
        else quaternion = a * (1 - t) - b * t;
        return Normalize(quaternion);
    }

    Quaternion Quaternion::LookRotation(Vector3 forward) {
        return LookRotation(forward, Vector3(0, 1, 0));
    }

    Quaternion Quaternion::LookRotation(Vector3 forward, Vector3 upwards) {
        forward = Vector3::Normalize(forward);
        upwards = Vector3::Normalize(upwards);
        constexpr float smallFloat = 0.0000000001f;
        if (Vector3::SqrMagnitude(forward) < smallFloat || Vector3::SqrMagnitude(upwards) < smallFloat) return Quaternion::identity;
        if (1 - fabs(Vector3::Dot(forward, upwards)) < smallFloat) return FromToRotation(Vector3::forward, forward);
        Vector3 right = Vector3::Normalize(Vector3::Cross(upwards, forward));
        upwards = Vector3::Cross(forward, right);
        Quaternion quaternion{};
        float radicand = right.x + upwards.y + forward.z;
        if (radicand > 0) {
            quaternion.w = sqrtf(1.0f + radicand) * 0.5f;
            float recip = 1.0f / (4.0f * quaternion.w);
            quaternion.x = (upwards.z - forward.y) * recip;
            quaternion.y = (forward.x - right.z) * recip;
            quaternion.z = (right.y - upwards.x) * recip;
        }
        else if (right.x >= upwards.y && right.x >= forward.z) {
            quaternion.x = sqrtf(1.0f + right.x - upwards.y - forward.z) * 0.5f;
            float recip = 1.0f / (4.0f * quaternion.x);
            quaternion.w = (upwards.z - forward.y) * recip;
            quaternion.z = (forward.x + right.z) * recip;
            quaternion.y = (right.y + upwards.x) * recip;
        }
        else if (upwards.y > forward.z) {
            quaternion.y = sqrtf(1.0f - right.x + upwards.y - forward.z) * 0.5f;
            float recip = 1.0f / (4.0f * quaternion.y);
            quaternion.z = (upwards.z + forward.y) * recip;
            quaternion.w = (forward.x - right.z) * recip;
            quaternion.x = (right.y + upwards.x) * recip;
        }
        else {
            quaternion.z = sqrtf(1.0f - right.x - upwards.y + forward.z) * 0.5f;
            float recip = 1.0f / (4.0f * quaternion.z);
            quaternion.y = (upwards.z + forward.y) * recip;
            quaternion.x = (forward.x + right.z) * recip;
            quaternion.w = (right.y - upwards.x) * recip;
        }
        return quaternion;
    }

    float Quaternion::Norm(Quaternion rotation) {
        return sqrtf(rotation.x * rotation.x + rotation.y * rotation.y + rotation.z * rotation.z + rotation.w * rotation.w);
    }

    Quaternion Quaternion::Normalize(Quaternion rotation) {
        return rotation / Norm(rotation);
    }

    Quaternion Quaternion::RotateTowards(Quaternion from, Quaternion to, float maxRadiansDelta) {
        float angle = Angle(from, to);
        if (angle == 0) return to;
        maxRadiansDelta = fmaxf(maxRadiansDelta, angle - (float) M_PI);
        float t = fminf(1, maxRadiansDelta / angle);
        return SlerpUnclamped(from, to, t);
    }

    Quaternion Quaternion::Slerp(Quaternion a, Quaternion b, float t) {
        if (t < 0) return Normalize(a);
        else if (t > 1) return Normalize(b);
        return SlerpUnclamped(a, b, t);
    }

    Quaternion Quaternion::SlerpUnclamped(Quaternion a, Quaternion b, float t) {
        float dot = Dot(a, b);
        if (dot < 0.f) {
            b = -b;
            dot = -dot;
        }

        if (dot < 0.95f) {
            float angle = acosf(dot);
            return (a * sinf(angle * (1.0f - t)) + b * sinf(angle * t)) * (1.0f / sinf(angle));
        }
        return LerpUnclamped(a, b, t);
    }

    void Quaternion::ToAngleAxis(Quaternion rotation, float &angle, Vector3 &axis) {
        if (rotation.w > 1) rotation = Normalize(rotation);
        angle = 2 * acosf(rotation.w);
        float s = sqrt(1 - rotation.w * rotation.w);
        if (s < 0.00001) {
            axis.x = 1;
            axis.y = 0;
            axis.z = 0;
        } else {
            axis.x = rotation.x / s;
            axis.y = rotation.y / s;
            axis.z = rotation.z / s;
        }
    }
    inline Vector3 Quaternion::RotateVectorByQuaternion(Quaternion lhs, Vector3 rhs) {
        float x = lhs.x * 2.f;
        float y = lhs.y * 2.f;
        float z = lhs.z * 2.f;
        float xx = lhs.x * x;
        float yy = lhs.y * y;
        float zz = lhs.z * z;
        float xy = lhs.x * y;
        float xz = lhs.x * z;
        float yz = lhs.y * z;
        float wx = lhs.w * x;
        float wy = lhs.w * y;
        float wz = lhs.w * z;

        Vector3 res;
        res.x = (1.0f - (yy + zz)) * rhs.x + (xy - wz) * rhs.y + (xz + wy) * rhs.z;
        res.y = (xy + wz) * rhs.x + (1.0f - (xx + zz)) * rhs.y + (yz - wx) * rhs.z;
        res.z = (xz - wy) * rhs.x + (yz + wx) * rhs.y + (1.0f - (xx + yy)) * rhs.z;

        return res;
    }

    struct Ray {
        Vector3 m_Origin{}, m_Direction{};

        inline std::string str() { return std::string("m_Origin: ") + m_Origin.str() + std::string(", m_Direction: ") + m_Direction.str(); }
    };

    struct RaycastHit {
        Vector3 point{}, normal{};
        unsigned int faceID{};
        float distance{};
        Vector2 UV{};
        int m_Collider{};
        void *GetCollider() const;

        inline std::string str() { return std::string("Point: ") + point.str() + std::string(", Normal: ") + normal.str() + std::string(", FaceID: ") + std::to_string(faceID) + std::string(", Distance: ") + std::to_string(distance) + std::string(", UV: ") + UV.str() + std::string(", m_Collider: ") + std::to_string((BNM_PTR)m_Collider); }
    };

    struct RaycastHit2D {
        Vector2 centroid, point, normal;
        float distance, fraction;
        int m_Collider{};
        void *GetCollider() const;

        inline std::string str() { return std::string("centroid: ") + centroid.str() + std::string(", point: ") + point.str() + std::string(", normal: ") + normal.str() + std::string(", Distance: ") + std::to_string(distance) + std::string(", fraction: ") + std::to_string(fraction) + std::string(", m_Collider: ") + std::to_string((BNM_PTR)m_Collider); }
    };

    struct Rect {
        union {
            struct { float x, y, w, h; };
            float data[4]{0.f, 0.f, 0.f, 0.f};
        };
        inline constexpr Rect() = default;
        inline constexpr Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}

        inline std::string str() { return std::string("x: ") + std::to_string(x) + std::string(", y: ") + std::to_string(y) + std::string(", w: ") + std::to_string(w) + std::string(", h: ") + std::to_string(h); }

        inline bool operator ==(const Rect& other) const { return x == other.x && y == other.y && w == other.w && h == other.h; }
        inline bool operator !=(const Rect& other) const = default;
    };

    struct Matrix4x4;

    inline static bool CompareApproximately(float f0, float f1, float epsilon = 0.000001f) {
        float dist = (f0 - f1);
        dist = abs(dist);
        return dist <= epsilon;
    }

    struct Matrix3x3 {
        float m_Data[9]{};
        inline Matrix3x3() = default;
        inline Matrix3x3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) { Get(0, 0) = m00; Get(1, 0) = m10; Get(2, 0) = m20; Get(0, 1) = m01; Get(1, 1) = m11; Get(2, 1) = m21; Get(0, 2) = m02; Get(1, 2) = m12; Get(2, 2) = m22; }
        Matrix3x3(const class Matrix4x4& m);

        inline float& Get(int row, int column) { return m_Data[row + (column * 3)]; }
        [[nodiscard]] inline const float& Get(int row, int column) const { return m_Data[row + (column * 3)]; }

        inline float& operator[](int row) { return m_Data[row]; }
        inline float operator[](int row) const { return m_Data[row]; }

        inline float* GetPtr() { return m_Data; }
        [[nodiscard]] inline const float* GetPtr() const { return m_Data; }

        [[nodiscard]] inline Vector3 GetColumn(int col) const { return {Get(0, col), Get(1, col), Get(2, col)}; }

        inline static void MultiplyMatrices3x3(const Matrix3x3* __restrict lhs, const Matrix3x3* __restrict rhs, Matrix3x3* __restrict res) {
            for (int i = 0; i < 3; ++i) {
                res->m_Data[i]    = lhs->m_Data[i] * rhs->m_Data[0]  + lhs->m_Data[i + 3] * rhs->m_Data[1]  + lhs->m_Data[i + 6] * rhs->m_Data[2];
                res->m_Data[i + 3]  = lhs->m_Data[i] * rhs->m_Data[3]  + lhs->m_Data[i + 3] * rhs->m_Data[4]  + lhs->m_Data[i + 6] * rhs->m_Data[5];
                res->m_Data[i + 6]  = lhs->m_Data[i] * rhs->m_Data[6]  + lhs->m_Data[i + 3] * rhs->m_Data[7]  + lhs->m_Data[i + 6] * rhs->m_Data[8];
            }
        }
        inline friend Matrix3x3 operator*(const Matrix3x3& lhs, const Matrix3x3& rhs) {
            Matrix3x3 temp;
            MultiplyMatrices3x3(&lhs, &rhs, &temp);
            return temp;
        }

        [[nodiscard]] inline Vector3 MultiplyVector3(const Vector3& inV) const {
            Vector3 res;
            res.x = m_Data[0] * inV.x + m_Data[3] * inV.y + m_Data[6] * inV.z;
            res.y = m_Data[1] * inV.x + m_Data[4] * inV.y + m_Data[7] * inV.z;
            res.z = m_Data[2] * inV.x + m_Data[5] * inV.y + m_Data[8] * inV.z;
            return res;
        }
        inline void MultiplyVector3(const Vector3& inV, Vector3& output) const {
            output.x = m_Data[0] * inV.x + m_Data[3] * inV.y + m_Data[6] * inV.z;
            output.y = m_Data[1] * inV.x + m_Data[4] * inV.y + m_Data[7] * inV.z;
            output.z = m_Data[2] * inV.x + m_Data[5] * inV.y + m_Data[8] * inV.z;
        }
        [[nodiscard]] inline Vector3 MultiplyPoint3(const Vector3& inV) const { return MultiplyVector3(inV); }
        [[nodiscard]] inline Vector3 MultiplyVector3Transpose(const Vector3& inV)  const {
            Vector3 res;
            res.x = Get(0, 0) * inV.x + Get(1, 0) * inV.y + Get(2, 0) * inV.z;
            res.y = Get(0, 1) * inV.x + Get(1, 1) * inV.y + Get(2, 1) * inV.z;
            res.z = Get(0, 2) * inV.x + Get(1, 2) * inV.y + Get(2, 2) * inV.z;
            return res;
        }
        [[nodiscard]] inline Vector3 MultiplyPoint3Transpose(const Vector3& inV) const { return MultiplyVector3Transpose(inV); }
        Matrix3x3& operator=(const class Matrix4x4& m);
        Matrix3x3& operator*=(const class Matrix4x4& inM);
        Matrix3x3& operator*=(const Matrix3x3& inM) {
            int i;
            for (i = 0; i < 3; i++) {
                float v[3] = {Get(i, 0), Get(i, 1), Get(i, 2)};
                Get(i, 0) = v[0] * inM.Get(0, 0) + v[1] * inM.Get(1, 0) + v[2] * inM.Get(2, 0);
                Get(i, 1) = v[0] * inM.Get(0, 1) + v[1] * inM.Get(1, 1) + v[2] * inM.Get(2, 1);
                Get(i, 2) = v[0] * inM.Get(0, 2) + v[1] * inM.Get(1, 2) + v[2] * inM.Get(2, 2);
            }
            return *this;
        }
        inline Matrix3x3& operator*=(float f) { for (float & i : m_Data) i *= f; return *this; }
        inline Matrix3x3& operator/=(float f) { return *this *= (1.0F / f); }
        bool Invert();
        [[nodiscard]] inline float GetDeterminant() const {
            float fCofactor0 = Get(0, 0) * Get(1, 1) * Get(2, 2);
            float fCofactor1 = Get(0, 1) * Get(1, 2) * Get(2, 0);
            float fCofactor2 = Get(0, 2) * Get(1, 0) * Get(2, 1);

            float fCofactor3 = Get(0, 2) * Get(1, 1) * Get(2, 0);
            float fCofactor4 = Get(0, 1) * Get(1, 0) * Get(2, 2);
            float fCofactor5 = Get(0, 0) * Get(1, 2) * Get(2, 1);

            return fCofactor0 + fCofactor1 + fCofactor2 - fCofactor3 - fCofactor4 - fCofactor5;
        }
        inline Matrix3x3& Transpose() {
            std::swap(Get(0, 1), Get(1, 0));
            std::swap(Get(0, 2), Get(2, 0));
            std::swap(Get(2, 1), Get(1, 2));
            return *this;
        }
        inline void InvertTranspose() {
            Invert();
            Transpose();
        }
        inline Matrix3x3& SetIdentity() {
            Get(0, 0) = 1.0F;  Get(0, 1) = 0.0F;  Get(0, 2) = 0.0F;
            Get(1, 0) = 0.0F;  Get(1, 1) = 1.0F;  Get(1, 2) = 0.0F;
            Get(2, 0) = 0.0F;  Get(2, 1) = 0.0F;  Get(2, 2) = 1.0F;
            return *this;
        }
        inline Matrix3x3& SetZero() {
            Get(0, 0) = 0.0F;  Get(0, 1) = 0.0F;  Get(0, 2) = 0.0F;
            Get(1, 0) = 0.0F;  Get(1, 1) = 0.0F;  Get(1, 2) = 0.0F;
            Get(2, 0) = 0.0F;  Get(2, 1) = 0.0F;  Get(2, 2) = 0.0F;
            return *this;
        }
        inline Matrix3x3& SetFromToRotation(const Vector3& from, const Vector3& to) {
            Vector3 v = Vector3::Cross(from, to);
            float e = Vector3::Dot(from, to);
            const float kEpsilon = 0.000001f;
            if (e > 1.0 - kEpsilon) {
                Get(0, 0) = 1.0; Get(0, 1) = 0.0; Get(0, 2) = 0.0;
                Get(1, 0) = 0.0; Get(1, 1) = 1.0; Get(1, 2) = 0.0;
                Get(2, 0) = 0.0; Get(2, 1) = 0.0; Get(2, 2) = 1.0;
            } else if (e < -1.0 + kEpsilon) {
                float invlen;
                float fxx, fyy, fzz, fxy, fxz, fyz;
                float uxx, uyy, uzz, uxy, uxz, uyz;
                float lxx, lyy, lzz, lxy, lxz, lyz;
                Vector3 left(0.0f, from[2], -from[1]);
                if (Vector3::Dot(left, left) < kEpsilon) left[0] = -from[2]; left[1] = 0.0; left[2] = from[0];

                invlen = 1.0f / sqrt(Vector3::Dot(left, left));
                left[0] *= invlen;
                left[1] *= invlen;
                left[2] *= invlen;
                Vector3 up = Vector3::Cross(left, from);
                fxx = -from[0] * from[0]; fyy = -from[1] * from[1]; fzz = -from[2] * from[2];
                fxy = -from[0] * from[1]; fxz = -from[0] * from[2]; fyz = -from[1] * from[2];

                uxx = up[0] * up[0]; uyy = up[1] * up[1]; uzz = up[2] * up[2];
                uxy = up[0] * up[1]; uxz = up[0] * up[2]; uyz = up[1] * up[2];

                lxx = -left[0] * left[0]; lyy = -left[1] * left[1]; lzz = -left[2] * left[2];
                lxy = -left[0] * left[1]; lxz = -left[0] * left[2]; lyz = -left[1] * left[2];

                Get(0, 0) = fxx + uxx + lxx; Get(0, 1) = fxy + uxy + lxy; Get(0, 2) = fxz + uxz + lxz;
                Get(1, 0) = Get(0, 1);   Get(1, 1) = fyy + uyy + lyy; Get(1, 2) = fyz + uyz + lyz;
                Get(2, 0) = Get(0, 2);   Get(2, 1) = Get(1, 2);   Get(2, 2) = fzz + uzz + lzz;
            } else {
                float hvx, hvz, hvxy, hvxz, hvyz;
                float h = (1.0f - e) / Vector3::Dot(v, v);
                hvx = h * v[0];
                hvz = h * v[2];
                hvxy = hvx * v[1];
                hvxz = hvx * v[2];
                hvyz = hvz * v[1];
                Get(0, 0) = e + hvx * v[0]; Get(0, 1) = hvxy - v[2];     Get(0, 2) = hvxz + v[1];
                Get(1, 0) = hvxy + v[2];  Get(1, 1) = e + h * v[1] * v[1]; Get(1, 2) = hvyz - v[0];
                Get(2, 0) = hvxz - v[1];  Get(2, 1) = hvyz + v[0];     Get(2, 2) = e + hvz * v[2];
            }
            return *this;
        }
        inline Matrix3x3& SetAxisAngle(const Vector3& rotationAxis, float radians) {
            GetRotMatrixNormVec(m_Data, rotationAxis.GetPtr(), radians);
            return *this;
        }
        inline Matrix3x3& SetBasis(const Vector3& inX, const Vector3& inY, const Vector3& inZ) {
            Get(0, 0) = inX[0];    Get(0, 1) = inY[0];    Get(0, 2) = inZ[0];
            Get(1, 0) = inX[1];    Get(1, 1) = inY[1];    Get(1, 2) = inZ[1];
            Get(2, 0) = inX[2];    Get(2, 1) = inY[2];    Get(2, 2) = inZ[2];
            return *this;
        }
        inline Matrix3x3& SetBasisTransposed(const Vector3& inX, const Vector3& inY, const Vector3& inZ) {
            Get(0, 0) = inX[0];    Get(1, 0) = inY[0];    Get(2, 0) = inZ[0];
            Get(0, 1) = inX[1];    Get(1, 1) = inY[1];    Get(2, 1) = inZ[1];
            Get(0, 2) = inX[2];    Get(1, 2) = inY[2];    Get(2, 2) = inZ[2];
            return *this;
        }
        inline Matrix3x3& SetScale(const Vector3& inScale) {
            Get(0, 0) = inScale[0];    Get(0, 1) = 0.0F;          Get(0, 2) = 0.0F;
            Get(1, 0) = 0.0F;          Get(1, 1) = inScale[1];    Get(1, 2) = 0.0F;
            Get(2, 0) = 0.0F;          Get(2, 1) = 0.0F;          Get(2, 2) = inScale[2];
            return *this;
        }
        inline Matrix3x3& Scale(const Vector3& inScale) {
            Get(0, 0) *= inScale[0];
            Get(1, 0) *= inScale[0];
            Get(2, 0) *= inScale[0];

            Get(0, 1) *= inScale[1];
            Get(1, 1) *= inScale[1];
            Get(2, 1) *= inScale[1];

            Get(0, 2) *= inScale[2];
            Get(1, 2) *= inScale[2];
            Get(2, 2) *= inScale[2];
            return *this;
        }
        inline bool IsIdentity(float threshold = Vector3::kEpsilon) {
            if (CompareApproximately(Get(0, 0), 1.0f, threshold) && CompareApproximately(Get(0, 1), 0.0f, threshold) && CompareApproximately(Get(0, 2), 0.0f, threshold) &&
                CompareApproximately(Get(1, 0), 0.0f, threshold) && CompareApproximately(Get(1, 1), 1.0f, threshold) && CompareApproximately(Get(1, 2), 0.0f, threshold) &&
                CompareApproximately(Get(2, 0), 0.0f, threshold) && CompareApproximately(Get(2, 1), 0.0f, threshold) && CompareApproximately(Get(2, 2), 1.0f, threshold))
                return true;
            return false;
        }
        inline static bool LookRotationToMatrix(const Vector3& viewVec, const Vector3& upVec, Matrix3x3* m) {
            Vector3 z = viewVec;
            float mag = Vector3::Magnitude(z);
            if (mag < Vector3::kEpsilon) {
                m->SetIdentity();
                return false;
            }
            z /= mag;

            Vector3 x = Vector3::Cross(upVec, z);
            mag = Vector3::Magnitude(x);
            if (mag < Vector3::kEpsilon) {
                m->SetIdentity();
                return false;
            }
            x /= mag;

            Vector3 y(Vector3::Cross(z, x));
            if (!CompareApproximately(Vector3::SqrMagnitude(y), 1.0F)) return false;

            m->SetBasis(x, y, z);
            return true;
        }
        inline static void EulerToMatrix(const Vector3& inV, Matrix3x3& matrix) {
            float cx = cos(inV.x);
            float sx = sin(inV.x);
            float cy = cos(inV.y);
            float sy = sin(inV.y);
            float cz = cos(inV.z);
            float sz = sin(inV.z);

            matrix.Get(0, 0) = cy * cz + sx * sy * sz;
            matrix.Get(0, 1) = cz * sx * sy - cy * sz;
            matrix.Get(0, 2) = cx * sy;

            matrix.Get(1, 0) = cx * sz;
            matrix.Get(1, 1) = cx * cz;
            matrix.Get(1, 2) = -sx;

            matrix.Get(2, 0) = -cz * sy + cy * sx * sz;
            matrix.Get(2, 1) = cy * cz * sx + sy * sz;
            matrix.Get(2, 2) = cx * cy;
        }
        inline static void GetRotMatrixNormVec(float* out, const float* inVec, float radians) {
            float s, c;
            float vx, vy, vz, xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

            s = sin(radians);
            c = cos(radians);

            vx = inVec[0];
            vy = inVec[1];
            vz = inVec[2];

#define M(row, col)  out[(row)*3 + col]
            xx = vx * vx;
            yy = vy * vy;
            zz = vz * vz;
            xy = vx * vy;
            yz = vy * vz;
            zx = vz * vx;
            xs = vx * s;
            ys = vy * s;
            zs = vz * s;
            one_c = 1.0F - c;

            M(0, 0) = (one_c * xx) + c;
            M(1, 0) = (one_c * xy) - zs;
            M(2, 0) = (one_c * zx) + ys;

            M(0, 1) = (one_c * xy) + zs;
            M(1, 1) = (one_c * yy) + c;
            M(2, 1) = (one_c * yz) - xs;

            M(0, 2) = (one_c * zx) - ys;
            M(1, 2) = (one_c * yz) + xs;
            M(2, 2) = (one_c * zz) + c;
#undef M
        }
        static void OrthoNormalize(Matrix3x3& matrix) {
            Vector3* c0 = (Vector3*)matrix.GetPtr() + 0;
            Vector3* c1 = (Vector3*)matrix.GetPtr() + 3;
            Vector3* c2 = (Vector3*)matrix.GetPtr() + 6;
            Vector3::OrthoNormalize(*c0, *c1, *c2);
        }

        static const Matrix3x3 zero;
        static const Matrix3x3 identity;
    };

#define MAT(m, r, c) (m)[(c)*4+(r)]
#define RETURN_ZERO do { for (int i=0;i<16;i++) out[i] = 0.0F; return false; } while(0)

    inline void QuaternionToMatrix(const Quaternion& q, struct Matrix4x4& m);

    struct FrustumPlanes {
        float left;
        float right;
        float bottom;
        float top;
        float zNear;
        float zFar;
    };

    inline bool InvertMatrix4x4_Full(const float* m, float* out);

    struct Matrix4x4 {
        float m_Data[16]{};
        enum class InitIdentity { kIdentity };

        Matrix4x4() = default;
        inline Matrix4x4(InitIdentity) { SetIdentity(); }
        inline Matrix4x4(const Matrix3x3 &other) {
            m_Data[0] = other.m_Data[0];
            m_Data[1] = other.m_Data[1];
            m_Data[2] = other.m_Data[2];
            m_Data[3] = 0.0F;

            m_Data[4] = other.m_Data[3];
            m_Data[5] = other.m_Data[4];
            m_Data[6] = other.m_Data[5];
            m_Data[7] = 0.0F;

            m_Data[8] = other.m_Data[6];
            m_Data[9] = other.m_Data[7];
            m_Data[10] = other.m_Data[8];
            m_Data[11] = 0.0F;

            m_Data[12] = 0.0F;
            m_Data[13] = 0.0F;
            m_Data[14] = 0.0F;
            m_Data[15] = 1.0F;
        }
        inline Matrix4x4(const float data[16]) {
            for (int i = 0; i < 16; i++)
            m_Data[i] = data[i];
        }

        float& Get(int row, int column) { return m_Data[row + (column * 4)]; }
        [[nodiscard]] const float& Get(int row, int column) const { return m_Data[row + (column * 4)]; }
        float* GetPtr() { return m_Data; }
        [[nodiscard]] const float* GetPtr() const { return m_Data; }

        float operator[](int index) const     { return m_Data[index]; }
        float& operator[](int index)          { return m_Data[index]; }
        inline static void MultiplyMatrices4x4(const Matrix4x4* __restrict lhs, const Matrix4x4* __restrict rhs, Matrix4x4* __restrict res) {
            for (int i = 0; i < 4; i++) {
                res->m_Data[i] = lhs->m_Data[i] * rhs->m_Data[0] + lhs->m_Data[i + 4] * rhs->m_Data[1] + lhs->m_Data[i + 8] * rhs->m_Data[2] + lhs->m_Data[i + 12] * rhs->m_Data[3];
                res->m_Data[i + 4] = lhs->m_Data[i] * rhs->m_Data[4] + lhs->m_Data[i + 4] * rhs->m_Data[5] + lhs->m_Data[i + 8] * rhs->m_Data[6] + lhs->m_Data[i + 12] * rhs->m_Data[7];
                res->m_Data[i + 8] = lhs->m_Data[i] * rhs->m_Data[8] + lhs->m_Data[i + 4] * rhs->m_Data[9] + lhs->m_Data[i + 8] * rhs->m_Data[10] + lhs->m_Data[i + 12] * rhs->m_Data[11];
                res->m_Data[i + 12] = lhs->m_Data[i] * rhs->m_Data[12] + lhs->m_Data[i + 4] * rhs->m_Data[13] + lhs->m_Data[i + 8] * rhs->m_Data[14] + lhs->m_Data[i + 12] * rhs->m_Data[15];
            }
        }
        inline Matrix4x4& operator*=(const Matrix4x4& inM) {
            Matrix4x4 tmp;
            MultiplyMatrices4x4(this, &inM, &tmp);
            return *this;
        }
        inline Matrix4x4& operator=(const Matrix3x3& other) {
            m_Data[0] = other.m_Data[0];
            m_Data[1] = other.m_Data[1];
            m_Data[2] = other.m_Data[2];
            m_Data[3] = 0.0F;

            m_Data[4] = other.m_Data[3];
            m_Data[5] = other.m_Data[4];
            m_Data[6] = other.m_Data[5];
            m_Data[7] = 0.0F;

            m_Data[8] = other.m_Data[6];
            m_Data[9] = other.m_Data[7];
            m_Data[10] = other.m_Data[8];
            m_Data[11] = 0.0F;

            m_Data[12] = 0.0F;
            m_Data[13] = 0.0F;
            m_Data[14] = 0.0F;
            m_Data[15] = 1.0F;
            return *this;
        }
        [[nodiscard]] inline Quaternion rotation() const {
            float m00 = m_Data[0], m01 = m_Data[4], m02 = m_Data[8];
            float m10 = m_Data[1], m11 = m_Data[5], m12 = m_Data[9];
            float m20 = m_Data[2], m21 = m_Data[6], m22 = m_Data[10];

            float len0 = std::sqrt(m00 * m00 + m01 * m01 + m02 * m02);
            float len1 = std::sqrt(m10 * m10 + m11 * m11 + m12 * m12);
            float len2 = std::sqrt(m20 * m20 + m21 * m21 + m22 * m22);

            if (len0 > 1e-6f) { m00 /= len0; m01 /= len0; m02 /= len0; }
            if (len1 > 1e-6f) { m10 /= len1; m11 /= len1; m12 /= len1; }
            if (len2 > 1e-6f) { m20 /= len2; m21 /= len2; m22 /= len2; }

            Quaternion q;
            float trace = m00 + m11 + m22;

            if (trace > 0.0f) {
                float s = std::sqrt(trace + 1.0f) * 2.0f;
                q.w = 0.25f * s;
                q.x = (m21 - m12) / s;
                q.y = (m02 - m20) / s;
                q.z = (m10 - m01) / s;
            } else if (m00 > m11 && m00 > m22) {
                float s = std::sqrt(1.0f + m00 - m11 - m22) * 2.0f;
                q.w = (m21 - m12) / s;
                q.x = 0.25f * s;
                q.y = (m01 + m10) / s;
                q.z = (m02 + m20) / s;
            } else if (m11 > m22) {
                float s = std::sqrt(1.0f + m11 - m00 - m22) * 2.0f;
                q.w = (m02 - m20) / s;
                q.x = (m01 + m10) / s;
                q.y = 0.25f * s;
                q.z = (m12 + m21) / s;
            } else {
                float s = std::sqrt(1.0f + m22 - m00 - m11) * 2.0f;
                q.w = (m10 - m01) / s;
                q.x = (m02 + m20) / s;
                q.y = (m12 + m21) / s;
                q.z = 0.25f * s;
            }

            return q;
        }
        [[nodiscard]] inline Vector3 MultiplyVector3(const Vector3& inV) const {
            Vector3 res;
            res.x = m_Data[0] * inV.x + m_Data[4] * inV.y + m_Data[8] * inV.z;
            res.y = m_Data[1] * inV.x + m_Data[5] * inV.y + m_Data[9] * inV.z;
            res.z = m_Data[2] * inV.x + m_Data[6] * inV.y + m_Data[10] * inV.z;
            return res;
        }
        inline void MultiplyVector3(const Vector3& inV, Vector3& output) const {
            output.x = m_Data[0] * inV.x + m_Data[4] * inV.y + m_Data[8] * inV.z;
            output.y = m_Data[1] * inV.x + m_Data[5] * inV.y + m_Data[9] * inV.z;
            output.z = m_Data[2] * inV.x + m_Data[6] * inV.y + m_Data[10] * inV.z;
        }
        inline bool PerspectiveMultiplyVector3(const Vector3& inV, Vector3& output) const {
            Vector3 res;
            float w;
            res.x = Get(0, 0) * inV.x + Get(0, 1) * inV.y + Get(0, 2) * inV.z;
            res.y = Get(1, 0) * inV.x + Get(1, 1) * inV.y + Get(1, 2) * inV.z;
            res.z = Get(2, 0) * inV.x + Get(2, 1) * inV.y + Get(2, 2) * inV.z;
            w     = Get(3, 0) * inV.x + Get(3, 1) * inV.y + Get(3, 2) * inV.z;
            if (abs(w) > 1.0e-7f) {
                float invW = 1.0f / w;
                output.x = res.x * invW;
                output.y = res.y * invW;
                output.z = res.z * invW;
                return true;
            }
            output.x = 0.0f;
            output.y = 0.0f;
            output.z = 0.0f;
            return false;
        }
        [[nodiscard]] inline Vector4 MultiplyVector4(const Vector4& inV) const {
            Vector4 res;
            MultiplyVector4(inV, res);
            return res;
        }
        inline void MultiplyVector4(const Vector4& inV, Vector4& output) const {
            output.x = m_Data[0] * inV.x + m_Data[4] * inV.y + m_Data[8] * inV.z + m_Data[12] * inV.w;
            output.y = m_Data[1] * inV.x + m_Data[5] * inV.y + m_Data[9] * inV.z + m_Data[13] * inV.w;
            output.z = m_Data[2] * inV.x + m_Data[6] * inV.y + m_Data[10] * inV.z + m_Data[14] * inV.w;
            output.w = m_Data[3] * inV.x + m_Data[7] * inV.y + m_Data[11] * inV.z + m_Data[15] * inV.w;
        }
        [[nodiscard]] inline Vector3 MultiplyPoint(const Vector3& point) const {
            Vector3 result;
            result.x = m_Data[0] * point.x + m_Data[4] * point.y + m_Data[8] * point.z + m_Data[12];
            result.y = m_Data[1] * point.x + m_Data[5] * point.y + m_Data[9] * point.z + m_Data[13];
            result.z = m_Data[2] * point.x + m_Data[6] * point.y + m_Data[10] * point.z + m_Data[14];
            float w = m_Data[3] * point.x + m_Data[7] * point.y + m_Data[11] * point.z + m_Data[15];
            if (std::abs(w) > 1e-6f) {
                float invW = 1.0f / w;
                result.x *= invW;
                result.y *= invW;
                result.z *= invW;
            }
            return result;
        }
        [[nodiscard]] inline Vector3 MultiplyPoint3x4(const Vector3& inV) const {
            Vector3 res;
            res.x = m_Data[0] * inV.x + m_Data[4] * inV.y + m_Data[8] * inV.z + m_Data[12];
            res.y = m_Data[1] * inV.x + m_Data[5] * inV.y + m_Data[9] * inV.z + m_Data[13];
            res.z = m_Data[2] * inV.x + m_Data[6] * inV.y + m_Data[10] * inV.z + m_Data[14];
            return res;
        }
        inline void MultiplyPoint3x4(const Vector3& inV, Vector3& output) const {
            output.x = m_Data[0] * inV.x + m_Data[4] * inV.y + m_Data[8] * inV.z + m_Data[12];
            output.y = m_Data[1] * inV.x + m_Data[5] * inV.y + m_Data[9] * inV.z + m_Data[13];
            output.z = m_Data[2] * inV.x + m_Data[6] * inV.y + m_Data[10] * inV.z + m_Data[14];
        }
        [[nodiscard]] inline Vector2 MultiplyPoint2(const Vector2& inV) const {
            Vector2 res;
            res.x = m_Data[0] * inV.x + m_Data[4] * inV.y + m_Data[12];
            res.y = m_Data[1] * inV.x + m_Data[5] * inV.y + m_Data[13];
            return res;
        }
        inline void MultiplyPoint2(const Vector2& inV, Vector2& output) const {
            output.x = m_Data[0] * inV.x + m_Data[4] * inV.y + m_Data[12];
            output.y = m_Data[1] * inV.x + m_Data[5] * inV.y + m_Data[13];
        }
        inline bool PerspectiveMultiplyPoint3(const Vector3& inV, Vector3& output) const {
            Vector3 res;
            float w;
            res.x = Get(0, 0) * inV.x + Get(0, 1) * inV.y + Get(0, 2) * inV.z + Get(0, 3);
            res.y = Get(1, 0) * inV.x + Get(1, 1) * inV.y + Get(1, 2) * inV.z + Get(1, 3);
            res.z = Get(2, 0) * inV.x + Get(2, 1) * inV.y + Get(2, 2) * inV.z + Get(2, 3);
            w     = Get(3, 0) * inV.x + Get(3, 1) * inV.y + Get(3, 2) * inV.z + Get(3, 3);
            if (abs(w) > 1.0e-7f) {
                float invW = 1.0f / w;
                output.x = res.x * invW;
                output.y = res.y * invW;
                output.z = res.z * invW;
                return true;
            }
            output.x = 0.0f;
            output.y = 0.0f;
            output.z = 0.0f;
            return false;
        }
        [[nodiscard]] inline Vector3 InverseMultiplyPoint3Affine(const Vector3& inV) const {
            Vector3 v(inV.x - Get(0, 3), inV.y - Get(1, 3), inV.z - Get(2, 3));
            Vector3 res;
            res.x = Get(0, 0) * v.x + Get(1, 0) * v.y + Get(2, 0) * v.z;
            res.y = Get(0, 1) * v.x + Get(1, 1) * v.y + Get(2, 1) * v.z;
            res.z = Get(0, 2) * v.x + Get(1, 2) * v.y + Get(2, 2) * v.z;
            return res;
        }
        [[nodiscard]] inline Vector3 InverseMultiplyVector3Affine(const Vector3& inV) const {
            Vector3 res;
            res.x = Get(0, 0) * inV.x + Get(1, 0) * inV.y + Get(2, 0) * inV.z;
            res.y = Get(0, 1) * inV.x + Get(1, 1) * inV.y + Get(2, 1) * inV.z;
            res.z = Get(0, 2) * inV.x + Get(1, 2) * inV.y + Get(2, 2) * inV.z;
            return res;
        }

        [[nodiscard]] inline bool IsIdentity(float threshold = Vector3::kEpsilon) const {
            if (CompareApproximately(Get(0, 0), 1.0f, threshold) && CompareApproximately(Get(0, 1), 0.0f, threshold) && CompareApproximately(Get(0, 2), 0.0f, threshold) && CompareApproximately(Get(0, 3), 0.0f, threshold) &&
                CompareApproximately(Get(1, 0), 0.0f, threshold) && CompareApproximately(Get(1, 1), 1.0f, threshold) && CompareApproximately(Get(1, 2), 0.0f, threshold) && CompareApproximately(Get(1, 3), 0.0f, threshold) &&
                CompareApproximately(Get(2, 0), 0.0f, threshold) && CompareApproximately(Get(2, 1), 0.0f, threshold) && CompareApproximately(Get(2, 2), 1.0f, threshold) && CompareApproximately(Get(2, 3), 0.0f, threshold) &&
                CompareApproximately(Get(3, 0), 0.0f, threshold) && CompareApproximately(Get(3, 1), 0.0f, threshold) && CompareApproximately(Get(3, 2), 0.0f, threshold) && CompareApproximately(Get(3, 3), 1.0f, threshold))
                return true;
            return false;
        }
        [[nodiscard]] bool IsPerspective() const { return (m_Data[3] != 0.0f || m_Data[7] != 0.0f || m_Data[11] != 0.0f || m_Data[15] != 1.0f); }
        [[nodiscard]] float MaxabsScale() const {
            float a = Vector3::SqrMagnitude(GetAxisX());
            a = std::max(a, Vector3::SqrMagnitude(GetAxisY()));
            a = std::max(a, Vector3::SqrMagnitude(GetAxisZ()));
            return sqrtf(a);
        }
        inline void RemoveScale() {
            SetAxisX(Vector3::Normalize(GetAxisX()));
            SetAxisY(Vector3::Normalize(GetAxisY()));
            SetAxisZ(Vector3::Normalize(GetAxisZ()));
        }

        [[nodiscard]] inline float GetDeterminant() const {
            double m00 = Get(0, 0);  double m01 = Get(0, 1);  double m02 = Get(0, 2);  double m03 = Get(0, 3);
            double m10 = Get(1, 0);  double m11 = Get(1, 1);  double m12 = Get(1, 2);  double m13 = Get(1, 3);
            double m20 = Get(2, 0);  double m21 = Get(2, 1);  double m22 = Get(2, 2);  double m23 = Get(2, 3);
            double m30 = Get(3, 0);  double m31 = Get(3, 1);  double m32 = Get(3, 2);  double m33 = Get(3, 3);

            double result =
                    m03 * m12 * m21 * m30 - m02 * m13 * m21 * m30 - m03 * m11 * m22 * m30 + m01 * m13 * m22 * m30 +
                    m02 * m11 * m23 * m30 - m01 * m12 * m23 * m30 - m03 * m12 * m20 * m31 + m02 * m13 * m20 * m31 +
                    m03 * m10 * m22 * m31 - m00 * m13 * m22 * m31 - m02 * m10 * m23 * m31 + m00 * m12 * m23 * m31 +
                    m03 * m11 * m20 * m32 - m01 * m13 * m20 * m32 - m03 * m10 * m21 * m32 + m00 * m13 * m21 * m32 +
                    m01 * m10 * m23 * m32 - m00 * m11 * m23 * m32 - m02 * m11 * m20 * m33 + m01 * m12 * m20 * m33 +
                    m02 * m10 * m21 * m33 - m00 * m12 * m21 * m33 - m01 * m10 * m22 * m33 + m00 * m11 * m22 * m33;
            return (float)result;
        }
        [[nodiscard]] inline float GetDeterminant2x2() const {
            return Get(0,0) * Get(1,1) - Get(0,1) * Get(1,0);
        }

        inline Matrix4x4& Invert_Full() {
            InvertMatrix4x4_Full(m_Data, m_Data);
            return *this;
        }

        inline static bool Invert_Full(const Matrix4x4 &inM, Matrix4x4 &outM) {
            return InvertMatrix4x4_Full(inM.m_Data, outM.m_Data);
        }

        inline static bool Invert_General3D(const Matrix4x4 &in, Matrix4x4 &out) {
            float pos, neg, t;
            float det;

            pos = neg = 0.0;
            t = MAT(in, 0, 0) * MAT(in, 1, 1) * MAT(in, 2, 2);
            if (t >= 0.0) pos += t;
            else neg += t;

            t = MAT(in, 1, 0) * MAT(in, 2, 1) * MAT(in, 0, 2);
            if (t >= 0.0) pos += t;
            else neg += t;

            t = MAT(in, 2, 0) * MAT(in, 0, 1) * MAT(in, 1, 2);
            if (t >= 0.0) pos += t;
            else neg += t;

            t = -MAT(in, 2, 0) * MAT(in, 1, 1) * MAT(in, 0, 2);
            if (t >= 0.0) pos += t;
            else neg += t;

            t = -MAT(in, 1, 0) * MAT(in, 0, 1) * MAT(in, 2, 2);
            if (t >= 0.0) pos += t;
            else neg += t;

            t = -MAT(in, 0, 0) * MAT(in, 2, 1) * MAT(in, 1, 2);
            if (t >= 0.0) pos += t;
            else neg += t;

            det = pos + neg;

            if (det * det < 1e-25)
            RETURN_ZERO;

            det = 1.0F / det;
            MAT(out, 0, 0) = ((MAT(in, 1, 1) * MAT(in, 2, 2) - MAT(in, 2, 1) * MAT(in, 1, 2)) * det);
            MAT(out, 0, 1) = (-(MAT(in, 0, 1) * MAT(in, 2, 2) - MAT(in, 2, 1) * MAT(in, 0, 2)) * det);
            MAT(out, 0, 2) = ((MAT(in, 0, 1) * MAT(in, 1, 2) - MAT(in, 1, 1) * MAT(in, 0, 2)) * det);
            MAT(out, 1, 0) = (-(MAT(in, 1, 0) * MAT(in, 2, 2) - MAT(in, 2, 0) * MAT(in, 1, 2)) * det);
            MAT(out, 1, 1) = ((MAT(in, 0, 0) * MAT(in, 2, 2) - MAT(in, 2, 0) * MAT(in, 0, 2)) * det);
            MAT(out, 1, 2) = (-(MAT(in, 0, 0) * MAT(in, 1, 2) - MAT(in, 1, 0) * MAT(in, 0, 2)) * det);
            MAT(out, 2, 0) = ((MAT(in, 1, 0) * MAT(in, 2, 1) - MAT(in, 2, 0) * MAT(in, 1, 1)) * det);
            MAT(out, 2, 1) = (-(MAT(in, 0, 0) * MAT(in, 2, 1) - MAT(in, 2, 0) * MAT(in, 0, 1)) * det);
            MAT(out, 2, 2) = ((MAT(in, 0, 0) * MAT(in, 1, 1) - MAT(in, 1, 0) * MAT(in, 0, 1)) * det);

            MAT(out, 0, 3) = -(MAT(in, 0, 3) * MAT(out, 0, 0) +
                MAT(in, 1, 3) * MAT(out, 0, 1) +
                MAT(in, 2, 3) * MAT(out, 0, 2));
            MAT(out, 1, 3) = -(MAT(in, 0, 3) * MAT(out, 1, 0) +
                MAT(in, 1, 3) * MAT(out, 1, 1) +
                MAT(in, 2, 3) * MAT(out, 1, 2));
            MAT(out, 2, 3) = -(MAT(in, 0, 3) * MAT(out, 2, 0) +
                MAT(in, 1, 3) * MAT(out, 2, 1) +
                MAT(in, 2, 3) * MAT(out, 2, 2));

            MAT(out, 3, 0) = 0.0f;
            MAT(out, 3, 1) = 0.0f;
            MAT(out, 3, 2) = 0.0f;
            MAT(out, 3, 3) = 1.0f;

            return true;
        }

        inline Matrix4x4& Transpose() {
            std::swap(Get(0, 1), Get(1, 0));
            std::swap(Get(0, 2), Get(2, 0));
            std::swap(Get(0, 3), Get(3, 0));
            std::swap(Get(1, 2), Get(2, 1));
            std::swap(Get(1, 3), Get(3, 1));
            std::swap(Get(2, 3), Get(3, 2));
            return *this;
        }

        inline Matrix4x4& SetIdentity() {
            Get(0, 0) = 1.0;   Get(0, 1) = 0.0;   Get(0, 2) = 0.0;   Get(0, 3) = 0.0;
            Get(1, 0) = 0.0;   Get(1, 1) = 1.0;   Get(1, 2) = 0.0;   Get(1, 3) = 0.0;
            Get(2, 0) = 0.0;   Get(2, 1) = 0.0;   Get(2, 2) = 1.0;   Get(2, 3) = 0.0;
            Get(3, 0) = 0.0;   Get(3, 1) = 0.0;   Get(3, 2) = 0.0;   Get(3, 3) = 1.0;
            return *this;
        }
        inline Matrix4x4& SetZero() {
            Get(0, 0) = 0.0;   Get(0, 1) = 0.0;   Get(0, 2) = 0.0;   Get(0, 3) = 0.0;
            Get(1, 0) = 0.0;   Get(1, 1) = 0.0;   Get(1, 2) = 0.0;   Get(1, 3) = 0.0;
            Get(2, 0) = 0.0;   Get(2, 1) = 0.0;   Get(2, 2) = 0.0;   Get(2, 3) = 0.0;
            Get(3, 0) = 0.0;   Get(3, 1) = 0.0;   Get(3, 2) = 0.0;   Get(3, 3) = 0.0;
            return *this;
        }
        inline Matrix4x4& SetPerspective(float fovy, float aspect, float zNear, float zFar) {
            constexpr float deg2Rad = M_PI / 180.f;

            float cotangent, deltaZ;
            float radians = deg2Rad * (fovy / 2.0f);
            cotangent = cos(radians) / sin(radians);
            deltaZ = zNear - zFar;

            Get(0, 0) = cotangent / aspect; Get(0, 1) = 0.0F;      Get(0, 2) = 0.0F;                    Get(0, 3) = 0.0F;
            Get(1, 0) = 0.0F;               Get(1, 1) = cotangent; Get(1, 2) = 0.0F;                    Get(1, 3) = 0.0F;
            Get(2, 0) = 0.0F;               Get(2, 1) = 0.0F;      Get(2, 2) = (zFar + zNear) / deltaZ; Get(2, 3) = 2.0F * zNear * zFar / deltaZ;
            Get(3, 0) = 0.0F;               Get(3, 1) = 0.0F;      Get(3, 2) = -1.0F;                   Get(3, 3) = 0.0F;

            return *this;
        }
        inline Matrix4x4& SetPerspectiveCotan(float cotanHalfFOV, float zNear, float zFar) {
            float deltaZ = zNear - zFar;

            Get(0, 0) = cotanHalfFOV;          Get(0, 1) = 0.0F;      Get(0, 2) = 0.0F;                    Get(0, 3) = 0.0F;
            Get(1, 0) = 0.0F;               Get(1, 1) = cotanHalfFOV; Get(1, 2) = 0.0F;                    Get(1, 3) = 0.0F;
            Get(2, 0) = 0.0F;               Get(2, 1) = 0.0F;      Get(2, 2) = (zFar + zNear) / deltaZ; Get(2, 3) = 2.0F * zNear * zFar / deltaZ;
            Get(3, 0) = 0.0F;               Get(3, 1) = 0.0F;      Get(3, 2) = -1.0F;                   Get(3, 3) = 0.0F;

            return *this;
        }
        inline Matrix4x4& SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar) {
            SetIdentity();

            float deltax = right - left;
            float deltay = top - bottom;
            float deltaz = zFar - zNear;

            Get(0, 0) = 2.0F / deltax;
            Get(0, 3) = -(right + left) / deltax;
            Get(1, 1) = 2.0F / deltay;
            Get(1, 3) = -(top + bottom) / deltay;
            Get(2, 2) = -2.0F / deltaz;
            Get(2, 3) = -(zFar + zNear) / deltaz;
            return *this;
        }
        inline Matrix4x4& SetFrustum(float left, float right, float bottom, float top, float nearval, float farval) {
            float x, y, a, b, c, d, e;

            x =  (2.0F * nearval)       / (right - left);
            y =  (2.0F * nearval)       / (top - bottom);
            a =  (right + left)         / (right - left);
            b =  (top + bottom)         / (top - bottom);
            c = -(farval + nearval)        / (farval - nearval);
            d = -(2.0f * farval * nearval) / (farval - nearval);
            e = -1.0f;

            Get(0, 0) = x;    Get(0, 1) = 0.0;  Get(0, 2) = a;   Get(0, 3) = 0.0;
            Get(1, 0) = 0.0;  Get(1, 1) = y;    Get(1, 2) = b;   Get(1, 3) = 0.0;
            Get(2, 0) = 0.0;  Get(2, 1) = 0.0;  Get(2, 2) = c;   Get(2, 3) = d;
            Get(3, 0) = 0.0;  Get(3, 1) = 0.0;  Get(3, 2) = e;  Get(3, 3) = 0.0;
            return *this;
        }
        inline Matrix4x4& AdjustDepthRange(float origNear, float newNear, float newFar) {
            if (IsPerspective()) {
                float x = Get(0, 0);
                float y = Get(1, 1);
                float w = Get(0, 2);
                float z = Get(1, 2);

                float r = ((2.0f * origNear) / x) * ((w + 1) * 0.5f);
                float t = ((2.0f * origNear) / y) * ((z + 1) * 0.5f);
                float l = ((2.0f * origNear) / x) * (((w + 1) * 0.5f) - 1.0f);
                float b = ((2.0f * origNear) / y) * (((z + 1) * 0.5f) - 1.0f);

                float ratio = (newNear / origNear);

                r *= ratio;
                t *= ratio;
                l *= ratio;
                b *= ratio;

                return SetFrustum(l, r, b, t, newNear, newFar);
            }
            float deltaz = newFar - newNear;
            Get(2, 2) = -2.0F / deltaz;
            Get(2, 3) = -(newFar + newNear) / deltaz;
            return *this;
        }
        [[nodiscard]] inline Vector3 GetAxisX() const { return {Get(0, 0), Get(1, 0), Get(2, 0)}; }
        [[nodiscard]] inline Vector3 GetAxisY() const { return {Get(0, 1), Get(1, 1), Get(2, 1)}; }
        [[nodiscard]] inline Vector3 GetAxisZ() const { return {Get(0, 2), Get(1, 2), Get(2, 2)}; }
        [[nodiscard]] inline Vector3 GetAxis(int axis) const { return {Get(0, axis), Get(1, axis), Get(2, axis)}; }
        [[nodiscard]] inline Vector3 GetPosition() const { return {Get(0, 3), Get(1, 3), Get(2, 3)}; }
        [[nodiscard]] inline Vector3 GetLossyScale() const {
            Vector3 result;
            result.x = Vector3::Magnitude(GetAxisX());
            result.y = Vector3::Magnitude(GetAxisY());
            result.z = Vector3::Magnitude(GetAxisZ());
            float determinant = Matrix3x3(*this).GetDeterminant();
            if (determinant < 0) result.x *= -1;
            return result;
        }
        [[nodiscard]] inline Vector4 GetRow(int row) const { return {Get(row, 0), Get(row, 1), Get(row, 2), Get(row, 3)}; }
        [[nodiscard]] inline Vector4 GetColumn(int col) const { return {Get(0, col), Get(1, col), Get(2, col), Get(3, col)}; }
        inline void SetAxisX(const Vector3& v) { Get(0, 0) = v.x; Get(1, 0) = v.y; Get(2, 0) = v.z; }
        inline void SetAxisY(const Vector3& v) { Get(0, 1) = v.x; Get(1, 1) = v.y; Get(2, 1) = v.z; }
        inline void SetAxisZ(const Vector3& v) { Get(0, 2) = v.x; Get(1, 2) = v.y; Get(2, 2) = v.z; }
        inline void SetAxis(int axis, const Vector3& v) { Get(0, axis) = v.x; Get(1, axis) = v.y; Get(2, axis) = v.z; }
        inline void SetPosition(const Vector3& v) { Get(0, 3) = v.x; Get(1, 3) = v.y; Get(2, 3) = v.z; }
        inline void SetRow(int row, const Vector4& v) { Get(row, 0) = v.x; Get(row, 1) = v.y; Get(row, 2) = v.z; Get(row, 3) = v.w; }
        inline void SetColumn(int col, const Vector4& v) { Get(0, col) = v.x; Get(1, col) = v.y; Get(2, col) = v.z; Get(3, col) = v.w; }
        inline Matrix4x4& SetTranslate(const Vector3& inTrans) {
            Get(0, 0) = 1.0;   Get(0, 1) = 0.0;   Get(0, 2) = 0.0;   Get(0, 3) = inTrans.x;
            Get(1, 0) = 0.0;   Get(1, 1) = 1.0;   Get(1, 2) = 0.0;   Get(1, 3) = inTrans.y;
            Get(2, 0) = 0.0;   Get(2, 1) = 0.0;   Get(2, 2) = 1.0;   Get(2, 3) = inTrans.z;
            Get(3, 0) = 0.0;   Get(3, 1) = 0.0;   Get(3, 2) = 0.0;   Get(3, 3) = 1.0;
            return *this;
        }
        inline Matrix4x4& SetBasis(const Vector3& inX, const Vector3& inY, const Vector3& inZ) {
            Get(0, 0) = inX[0];    Get(0, 1) = inY[0];    Get(0, 2) = inZ[0];    Get(0, 3) = 0.0;
            Get(1, 0) = inX[1];    Get(1, 1) = inY[1];    Get(1, 2) = inZ[1];    Get(1, 3) = 0.0;
            Get(2, 0) = inX[2];    Get(2, 1) = inY[2];    Get(2, 2) = inZ[2];    Get(2, 3) = 0.0;
            Get(3, 0) = 0.0;       Get(3, 1) = 0.0;       Get(3, 2) = 0.0;       Get(3, 3) = 1.0;
            return *this;
        }
        inline Matrix4x4& SetBasisTransposed(const Vector3& inX, const Vector3& inY, const Vector3& inZ) {
            Get(0, 0) = inX[0];    Get(1, 0) = inY[0];    Get(2, 0) = inZ[0];    Get(3, 0) = 0.0;
            Get(0, 1) = inX[1];    Get(1, 1) = inY[1];    Get(2, 1) = inZ[1];    Get(3, 1) = 0.0;
            Get(0, 2) = inX[2];    Get(1, 2) = inY[2];    Get(2, 2) = inZ[2];    Get(3, 2) = 0.0;
            Get(0, 3) = 0.0;       Get(1, 3) = 0.0;       Get(2, 3) = 0.0;       Get(3, 3) = 1.0;
            return *this;
        }
        inline Matrix4x4& SetScale(const Vector3& inScale){
            Get(0, 0) = inScale.x;    Get(0, 1) = 0.0;       Get(0, 2) = 0.0;       Get(0, 3) = 0.0;
            Get(1, 0) = 0.0;       Get(1, 1) = inScale.y;    Get(1, 2) = 0.0;       Get(1, 3) = 0.0;
            Get(2, 0) = 0.0;       Get(2, 1) = 0.0;       Get(2, 2) = inScale.z;    Get(2, 3) = 0.0;
            Get(3, 0) = 0.0;       Get(3, 1) = 0.0;       Get(3, 2) = 0.0;       Get(3, 3) = 1.0;
            return *this;
        }
        inline Matrix4x4& SetScaleAndPosition(const Vector3& inScale, const Vector3& inPosition) {
            Get(0, 0) = inScale[0];    Get(0, 1) = 0.0;           Get(0, 2) = 0.0;           Get(0, 3) = inPosition[0];
            Get(1, 0) = 0.0;           Get(1, 1) = inScale[1];    Get(1, 2) = 0.0;           Get(1, 3) = inPosition[1];
            Get(2, 0) = 0.0;           Get(2, 1) = 0.0;           Get(2, 2) = inScale[2];    Get(2, 3) = inPosition[2];
            Get(3, 0) = 0.0;           Get(3, 1) = 0.0;           Get(3, 2) = 0.0;           Get(3, 3) = 1.0;
            return *this;
        }
        inline Matrix4x4& SetPositionAndOrthoNormalBasis(const Vector3& inPosition, const Vector3& inX, const Vector3& inY, const Vector3& inZ) {
            Get(0, 0) = inX[0];    Get(0, 1) = inY[0];    Get(0, 2) = inZ[0];    Get(0, 3) = inPosition[0];
            Get(1, 0) = inX[1];    Get(1, 1) = inY[1];    Get(1, 2) = inZ[1];    Get(1, 3) = inPosition[1];
            Get(2, 0) = inX[2];    Get(2, 1) = inY[2];    Get(2, 2) = inZ[2];    Get(2, 3) = inPosition[2];
            Get(3, 0) = 0.0;       Get(3, 1) = 0.0;       Get(3, 2) = 0.0;       Get(3, 3) = 1.0;
            return *this;
        }
        inline Matrix4x4& Translate(const Vector3& inTrans) {
            Get(0, 3) = Get(0, 0) * inTrans[0] + Get(0, 1) * inTrans[1] + Get(0, 2) * inTrans[2] + Get(0, 3);
            Get(1, 3) = Get(1, 0) * inTrans[0] + Get(1, 1) * inTrans[1] + Get(1, 2) * inTrans[2] + Get(1, 3);
            Get(2, 3) = Get(2, 0) * inTrans[0] + Get(2, 1) * inTrans[1] + Get(2, 2) * inTrans[2] + Get(2, 3);
            Get(3, 3) = Get(3, 0) * inTrans[0] + Get(3, 1) * inTrans[1] + Get(3, 2) * inTrans[2] + Get(3, 3);
            return *this;
        }
        inline Matrix4x4& Scale(const Vector3& inScale) {
            Get(0, 0) *= inScale[0];
            Get(1, 0) *= inScale[0];
            Get(2, 0) *= inScale[0];
            Get(3, 0) *= inScale[0];

            Get(0, 1) *= inScale[1];
            Get(1, 1) *= inScale[1];
            Get(2, 1) *= inScale[1];
            Get(3, 1) *= inScale[1];

            Get(0, 2) *= inScale[2];
            Get(1, 2) *= inScale[2];
            Get(2, 2) *= inScale[2];
            Get(3, 2) *= inScale[2];
            return *this;
        }

        inline Matrix4x4& SetFromToRotation(const Vector3& from, const Vector3& to) {
            Matrix3x3 mat;
            mat.SetFromToRotation(from, to);
            return *this;
        }

        inline void SetTR(const Vector3& pos, const Quaternion& q) {
            QuaternionToMatrix(q, *this);
            m_Data[12] = pos[0];
            m_Data[13] = pos[1];
            m_Data[14] = pos[2];
        }
        inline void SetTRS(const Vector3& pos, const Quaternion& q, const Vector3& s) {
            QuaternionToMatrix(q, *this);

            m_Data[0] *= s[0];
            m_Data[1] *= s[0];
            m_Data[2] *= s[0];

            m_Data[4] *= s[1];
            m_Data[5] *= s[1];
            m_Data[6] *= s[1];

            m_Data[8] *= s[2];
            m_Data[9] *= s[2];
            m_Data[10] *= s[2];

            m_Data[12] = pos[0];
            m_Data[13] = pos[1];
            m_Data[14] = pos[2];
        }
        inline void SetTRInverse(const Vector3& pos, const Quaternion& q) {
            QuaternionToMatrix(Quaternion::Inverse(q), *this);
            Translate(Vector3(-pos[0], -pos[1], -pos[2]));
        }
        [[nodiscard]] FrustumPlanes DecomposeProjection() const {
            FrustumPlanes planes{};
            if (IsPerspective()) {
                planes.zNear = Get(2, 3) / (Get(2, 2) - 1.0f);
                planes.zFar = Get(2, 3) / (Get(2, 2) + 1.0f);
                planes.right = planes.zNear * (1.0f + Get(0, 2)) / Get(0, 0);
                planes.left = planes.zNear * (-1.0f + Get(0, 2)) / Get(0, 0);
                planes.top = planes.zNear  * (1.0f + Get(1, 2)) / Get(1, 1);
                planes.bottom = planes.zNear  * (-1.0f + Get(1, 2)) / Get(1, 1);
                return planes;
            }
            planes.zNear = (Get(2, 3) + 1.0f) / Get(2, 2);
            planes.zFar =  (Get(2, 3) - 1.0f) / Get(2, 2);
            planes.right = (1.0f - Get(0, 3)) / Get(0, 0);
            planes.left = (-1.0f - Get(0, 3)) / Get(0, 0);
            planes.top = (1.0f - Get(1, 3)) / Get(1, 1);
            planes.bottom = (-1.0f - Get(1, 3)) / Get(1, 1);
            return planes;
        }
        static const Matrix4x4 identity;
        [[nodiscard]] inline bool ValidTRS() const {
            return Get(3, 0) == 0 && Get(3, 1) == 0 && Get(3, 2) == 0 && fabs(Get(3, 3)) == 1;
        }
        inline static Matrix4x4 Rotate(Quaternion q) {
            float x = q.x * 2.0f;
            float y = q.y * 2.0f;
            float z = q.z * 2.0f;
            float xx = q.x * x;
            float yy = q.y * y;
            float zz = q.z * z;
            float xy = q.x * y;
            float xz = q.x * z;
            float yz = q.y * z;
            float wx = q.w * x;
            float wy = q.w * y;
            float wz = q.w * z;

            Matrix4x4 m;
            m.m_Data[0] = 1.0f - (yy + zz); m.m_Data[1] = xy + wz; m.m_Data[2] = xz - wy; m.m_Data[3] = 0.0f;
            m.m_Data[4] = xy - wz; m.m_Data[5] = 1.0f - (xx + zz); m.m_Data[6] = yz + wx; m.m_Data[7] = 0.0f;
            m.m_Data[8] = xz + wy; m.m_Data[9] = yz - wx; m.m_Data[10] = 1.0f - (xx + yy); m.m_Data[11] = 0.0f;
            m.m_Data[12] = 0.0f; m.m_Data[13] = 0.0f; m.m_Data[14] = 0.0F; m.m_Data[15] = 1.0f;
            return m;
        }
    };
    inline Matrix4x4 operator *(const Matrix4x4 &lhs, const Matrix4x4 &rhs) {
        Matrix4x4 res;
        res.m_Data[0] = lhs.m_Data[0] * rhs.m_Data[0] + lhs.m_Data[4] * rhs.m_Data[1] + lhs.m_Data[8] * rhs.m_Data[2] + lhs.m_Data[12] * rhs.m_Data[3];
        res.m_Data[4] = lhs.m_Data[0] * rhs.m_Data[4] + lhs.m_Data[4] * rhs.m_Data[5] + lhs.m_Data[8] * rhs.m_Data[6] + lhs.m_Data[12] * rhs.m_Data[7];
        res.m_Data[8] = lhs.m_Data[0] * rhs.m_Data[8] + lhs.m_Data[4] * rhs.m_Data[9] + lhs.m_Data[8] * rhs.m_Data[10] + lhs.m_Data[12] * rhs.m_Data[11];
        res.m_Data[12] = lhs.m_Data[0] * rhs.m_Data[12] + lhs.m_Data[4] * rhs.m_Data[13] + lhs.m_Data[8] * rhs.m_Data[14] + lhs.m_Data[12] * rhs.m_Data[15];
        res.m_Data[1] = lhs.m_Data[1] * rhs.m_Data[0] + lhs.m_Data[5] * rhs.m_Data[1] + lhs.m_Data[9] * rhs.m_Data[2] + lhs.m_Data[13] * rhs.m_Data[3];
        res.m_Data[5] = lhs.m_Data[1] * rhs.m_Data[4] + lhs.m_Data[5] * rhs.m_Data[5] + lhs.m_Data[9] * rhs.m_Data[6] + lhs.m_Data[13] * rhs.m_Data[7];
        res.m_Data[9] = lhs.m_Data[1] * rhs.m_Data[8] + lhs.m_Data[5] * rhs.m_Data[9] + lhs.m_Data[9] * rhs.m_Data[10] + lhs.m_Data[13] * rhs.m_Data[11];
        res.m_Data[13] = lhs.m_Data[1] * rhs.m_Data[12] + lhs.m_Data[5] * rhs.m_Data[13] + lhs.m_Data[9] * rhs.m_Data[14] + lhs.m_Data[13] * rhs.m_Data[15];
        res.m_Data[2] = lhs.m_Data[2] * rhs.m_Data[0] + lhs.m_Data[6] * rhs.m_Data[1] + lhs.m_Data[10] * rhs.m_Data[2] + lhs.m_Data[14] * rhs.m_Data[3];
        res.m_Data[6] = lhs.m_Data[2] * rhs.m_Data[4] + lhs.m_Data[6] * rhs.m_Data[5] + lhs.m_Data[10] * rhs.m_Data[6] + lhs.m_Data[14] * rhs.m_Data[7];
        res.m_Data[10] = lhs.m_Data[2] * rhs.m_Data[8] + lhs.m_Data[6] * rhs.m_Data[9] + lhs.m_Data[10] * rhs.m_Data[10] + lhs.m_Data[14] * rhs.m_Data[11];
        res.m_Data[14] = lhs.m_Data[2] * rhs.m_Data[12] + lhs.m_Data[6] * rhs.m_Data[13] + lhs.m_Data[10] * rhs.m_Data[14] + lhs.m_Data[14] * rhs.m_Data[15];
        res.m_Data[3] = lhs.m_Data[3] * rhs.m_Data[0] + lhs.m_Data[7] * rhs.m_Data[1] + lhs.m_Data[11] * rhs.m_Data[2] + lhs.m_Data[15] * rhs.m_Data[3];
        res.m_Data[7] = lhs.m_Data[3] * rhs.m_Data[4] + lhs.m_Data[7] * rhs.m_Data[5] + lhs.m_Data[11] * rhs.m_Data[6] + lhs.m_Data[15] * rhs.m_Data[7];
        res.m_Data[11] = lhs.m_Data[3] * rhs.m_Data[8] + lhs.m_Data[7] * rhs.m_Data[9] + lhs.m_Data[11] * rhs.m_Data[10] + lhs.m_Data[15] * rhs.m_Data[11];
        res.m_Data[15] = lhs.m_Data[3] * rhs.m_Data[12] + lhs.m_Data[7] * rhs.m_Data[13] + lhs.m_Data[11] * rhs.m_Data[14] + lhs.m_Data[15] * rhs.m_Data[15];
        return res;
    }
    inline void QuaternionToMatrix(const Quaternion& q, Matrix4x4& m) {
        float x = q.x * 2.0F;
        float y = q.y * 2.0F;
        float z = q.z * 2.0F;
        float xx = q.x * x;
        float yy = q.y * y;
        float zz = q.z * z;
        float xy = q.x * y;
        float xz = q.x * z;
        float yz = q.y * z;
        float wx = q.w * x;
        float wy = q.w * y;
        float wz = q.w * z;

        m.m_Data[0] = 1.0f - (yy + zz);
        m.m_Data[1] = xy + wz;
        m.m_Data[2] = xz - wy;
        m.m_Data[3] = 0.0F;

        m.m_Data[4] = xy - wz;
        m.m_Data[5] = 1.0f - (xx + zz);
        m.m_Data[6] = yz + wx;
        m.m_Data[7] = 0.0F;

        m.m_Data[8]  = xz + wy;
        m.m_Data[9]  = yz - wx;
        m.m_Data[10] = 1.0f - (xx + yy);
        m.m_Data[11] = 0.0F;

        m.m_Data[12] = 0.0F;
        m.m_Data[13] = 0.0F;
        m.m_Data[14] = 0.0F;
        m.m_Data[15] = 1.0F;
    }

    inline void MultiplyMatrices3x4(const Matrix4x4& lhs, const Matrix4x4& rhs, Matrix4x4& res) {
        for (int i = 0; i < 3; i++) {
            res.m_Data[i]    = lhs.m_Data[i] * rhs.m_Data[0]  + lhs.m_Data[i + 4] * rhs.m_Data[1]  + lhs.m_Data[i + 8] * rhs.m_Data[2];//  + lhs.m_Data[i+12] * rhs.m_Data[3];
            res.m_Data[i + 4]  = lhs.m_Data[i] * rhs.m_Data[4]  + lhs.m_Data[i + 4] * rhs.m_Data[5]  + lhs.m_Data[i + 8] * rhs.m_Data[6];//  + lhs.m_Data[i+12] * rhs.m_Data[7];
            res.m_Data[i + 8]  = lhs.m_Data[i] * rhs.m_Data[8]  + lhs.m_Data[i + 4] * rhs.m_Data[9]  + lhs.m_Data[i + 8] * rhs.m_Data[10];// + lhs.m_Data[i+12] * rhs.m_Data[11];
            res.m_Data[i + 12] = lhs.m_Data[i] * rhs.m_Data[12] + lhs.m_Data[i + 4] * rhs.m_Data[13] + lhs.m_Data[i + 8] * rhs.m_Data[14] + lhs.m_Data[i + 12];// * rhs.m_Data[15];
        }

        res.m_Data[3]  = 0.0f;
        res.m_Data[7]  = 0.0f;
        res.m_Data[11] = 0.0f;
        res.m_Data[15] = 1.0f;
    }

    inline void MultiplyMatrices2D(const Matrix4x4& lhs, const Matrix4x4& rhs, Matrix4x4& res) {
        res.m_Data[0] = lhs.m_Data[0] * rhs.m_Data[0]  + lhs.m_Data[0 + 4] * rhs.m_Data[1]  + lhs.m_Data[0 + 8] * rhs.m_Data[2];
        res.m_Data[4] = lhs.m_Data[0] * rhs.m_Data[4]  + lhs.m_Data[0 + 4] * rhs.m_Data[5]  + lhs.m_Data[0 + 8] * rhs.m_Data[6];
        res.m_Data[12] = lhs.m_Data[0] * rhs.m_Data[12] + lhs.m_Data[0 + 4] * rhs.m_Data[13] + lhs.m_Data[0 + 8] * rhs.m_Data[14] + lhs.m_Data[0 + 12];
        res.m_Data[1]    = lhs.m_Data[1] * rhs.m_Data[0]  + lhs.m_Data[1 + 4] * rhs.m_Data[1]  + lhs.m_Data[1 + 8] * rhs.m_Data[2];
        res.m_Data[5]  = lhs.m_Data[1] * rhs.m_Data[4]  + lhs.m_Data[1 + 4] * rhs.m_Data[5]  + lhs.m_Data[1 + 8] * rhs.m_Data[6];
        res.m_Data[13] = lhs.m_Data[1] * rhs.m_Data[12] + lhs.m_Data[1 + 4] * rhs.m_Data[13] + lhs.m_Data[1 + 8] * rhs.m_Data[14] + lhs.m_Data[1 + 12];
        res.m_Data[2]    = lhs.m_Data[2] * rhs.m_Data[0]  + lhs.m_Data[2 + 4] * rhs.m_Data[1]  + lhs.m_Data[2 + 8] * rhs.m_Data[2];
        res.m_Data[14] = lhs.m_Data[2] * rhs.m_Data[12] + lhs.m_Data[2 + 4] * rhs.m_Data[13] + lhs.m_Data[2 + 8] * rhs.m_Data[14] + lhs.m_Data[2 + 12];
        res.m_Data[3]  = 0;
        res.m_Data[6]  = 0;
        res.m_Data[7]  = 0;
        res.m_Data[8]  = 0;
        res.m_Data[9]  = 0;
        res.m_Data[10]  = 1.0f;
        res.m_Data[11] = 0;
        res.m_Data[15] = 1.0f;
    }

   inline float ComputeUniformScale(const Matrix4x4& matrix) { return Vector3::Magnitude(matrix.GetAxisX()); }

#define SWAP_ROWS(a, b) do { float *_tmp = a; (a)=(b); (b)=_tmp; } while(false)
   inline bool InvertMatrix4x4_Full(const float* m, float* out) {
        float wtmp[4][8];
        float m0, m1, m2, m3, s;
        float *r0, *r1, *r2, *r3;

        r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

        r0[0] = MAT(m, 0, 0); r0[1] = MAT(m, 0, 1);
        r0[2] = MAT(m, 0, 2); r0[3] = MAT(m, 0, 3);
        r0[4] = 1.0; r0[5] = r0[6] = r0[7] = 0.0;

        r1[0] = MAT(m, 1, 0); r1[1] = MAT(m, 1, 1);
        r1[2] = MAT(m, 1, 2); r1[3] = MAT(m, 1, 3);
        r1[5] = 1.0; r1[4] = r1[6] = r1[7] = 0.0;

        r2[0] = MAT(m, 2, 0); r2[1] = MAT(m, 2, 1);
        r2[2] = MAT(m, 2, 2); r2[3] = MAT(m, 2, 3);
        r2[6] = 1.0; r2[4] = r2[5] = r2[7] = 0.0;

        r3[0] = MAT(m, 3, 0); r3[1] = MAT(m, 3, 1);
        r3[2] = MAT(m, 3, 2); r3[3] = MAT(m, 3, 3);
        r3[7] = 1.0; r3[4] = r3[5] = r3[6] = 0.0;

        if (abs(r3[0]) > abs(r2[0])) SWAP_ROWS(r3, r2);
        if (abs(r2[0]) > abs(r1[0])) SWAP_ROWS(r2, r1);
        if (abs(r1[0]) > abs(r0[0])) SWAP_ROWS(r1, r0);
        if (0.0F == r0[0]) RETURN_ZERO;

        m1 = r1[0] / r0[0]; m2 = r2[0] / r0[0]; m3 = r3[0] / r0[0];
        s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
        s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
        s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
        s = r0[4]; if (s != 0.0F) r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s;
        s = r0[5]; if (s != 0.0F) r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s;
        s = r0[6]; if (s != 0.0F) r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s;
        s = r0[7]; if (s != 0.0F) r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s;
        if (abs(r3[1]) > abs(r2[1])) SWAP_ROWS(r3, r2);
        if (abs(r2[1]) > abs(r1[1])) SWAP_ROWS(r2, r1);
        if (0.0F == r1[1]) RETURN_ZERO;
        m2 = r2[1] / r1[1]; m3 = r3[1] / r1[1];
        r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
        r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
        s = r1[4]; if (0.0F != s) r2[4] -= m2 * s; r3[4] -= m3 * s;
        s = r1[5]; if (0.0F != s) r2[5] -= m2 * s; r3[5] -= m3 * s;
        s = r1[6]; if (0.0F != s) r2[6] -= m2 * s; r3[6] -= m3 * s;
        s = r1[7]; if (0.0F != s) r2[7] -= m2 * s; r3[7] -= m3 * s;
        if (abs(r3[2]) > abs(r2[2])) SWAP_ROWS(r3, r2);
        if (0.0F == r2[2]) RETURN_ZERO;
        m3 = r3[2] / r2[2];
        r3[3] -= m3 * r2[3]; r3[4] -= m3 * r2[4];
        r3[5] -= m3 * r2[5]; r3[6] -= m3 * r2[6];
        r3[7] -= m3 * r2[7];
        if (0.0F == r3[3]) RETURN_ZERO;
        s = 1.0F / r3[3];
        r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;
        m2 = r2[3];
        s  = 1.0F / r2[2];
        r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
        r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
        m1 = r1[3];
        r1[4] -= r3[4] * m1; r1[5] -= r3[5] * m1, r1[6] -= r3[6] * m1; r1[7] -= r3[7] * m1;
        m0 = r0[3];
        r0[4] -= r3[4] * m0; r0[5] -= r3[5] * m0, r0[6] -= r3[6] * m0; r0[7] -= r3[7] * m0;
        m1 = r1[2];
        s  = 1.0F / r1[1];
        r1[4] = s * (r1[4] - r2[4] * m1); r1[5] = s * (r1[5] - r2[5] * m1), r1[6] = s * (r1[6] - r2[6] * m1); r1[7] = s * (r1[7] - r2[7] * m1);
        m0 = r0[2];
        r0[4] -= r2[4] * m0; r0[5] -= r2[5] * m0, r0[6] -= r2[6] * m0; r0[7] -= r2[7] * m0;
        m0 = r0[1];
        s  = 1.0F / r0[0];
        r0[4] = s * (r0[4] - r1[4] * m0); r0[5] = s * (r0[5] - r1[5] * m0), r0[6] = s * (r0[6] - r1[6] * m0); r0[7] = s * (r0[7] - r1[7] * m0);
        MAT(out, 0, 0) = r0[4]; MAT(out, 0, 1) = r0[5], MAT(out, 0, 2) = r0[6]; MAT(out, 0, 3) = r0[7];
        MAT(out, 1, 0) = r1[4]; MAT(out, 1, 1) = r1[5], MAT(out, 1, 2) = r1[6]; MAT(out, 1, 3) = r1[7];
        MAT(out, 2, 0) = r2[4]; MAT(out, 2, 1) = r2[5], MAT(out, 2, 2) = r2[6]; MAT(out, 2, 3) = r2[7];
        MAT(out, 3, 0) = r3[4]; MAT(out, 3, 1) = r3[5], MAT(out, 3, 2) = r3[6]; MAT(out, 3, 3) = r3[7];
        return true;
    }

#undef SWAP_ROWS
#undef MAT
#undef RETURN_ZERO

    template<class T>
    inline T* Stride(T* p, size_t offset) { return reinterpret_cast<T*>((char*)p + offset); }

    inline bool CompareApproximately(const Matrix4x4& lhs, const Matrix4x4& rhs, float dist) {
        for (int i = 0; i < 16; i++) if (!CompareApproximately(lhs[i], rhs[i], dist)) return false;
        return true;
    }
    inline void TransformPoints3x3(const Matrix4x4& matrix, const Vector3* in, Vector3* out, int count) {
        auto m = Matrix3x3(matrix);
        for (int i = 0; i < count; i++) out[i] = m.MultiplyPoint3(in[i]);
    }
    inline void TransformPoints3x4(const Matrix4x4& matrix, const Vector3* in, Vector3* out, int count) {
        for (int i = 0; i < count; i++) out[i] = matrix.MultiplyPoint3x4(in[i]);
    }
    inline void TransformPoints3x3(const Matrix4x4& matrix, const Vector3* in, size_t inStride, Vector3* out, size_t outStride, int count) {
        auto m = Matrix3x3(matrix);
        for (int i = 0; i < count; ++i, in = Stride(in, inStride), out = Stride(out, outStride)) *out = m.MultiplyPoint3(*in);
    }
    inline void TransformPoints3x4(const Matrix4x4& matrix, const Vector3* in, size_t inStride, Vector3* out, size_t outStride, int count) {
        for (int i = 0; i < count; ++i, in = Stride(in, inStride), out = Stride(out, outStride)) *out = matrix.MultiplyPoint3x4(*in);
    }

}
}
}

namespace BNM {
namespace UnityEngine {

struct Object : public BNM::IL2CPP::Il2CppObject {
    constexpr Object() : BNM::IL2CPP::Il2CppObject({}) {}
    BNM_INT_PTR m_CachedPtr = 0;
    [[nodiscard]] inline bool IsValid() const { return BNM::Utils::CheckForNull(this) && m_CachedPtr; }
    [[nodiscard]] inline bool Alive() const { return IsValid(); }
    inline bool Same(void *object) const { return Same((Object *) object); }
    inline bool Same(Object *object) const { return (!Alive() && !object->Alive()) || (Alive() && object->Alive() && m_CachedPtr == object->m_CachedPtr); }
};

struct MonoBehaviour : public Object {
    void *m_CancellationTokenSource{};
};

template<typename T>
inline bool IsUnityObjectAlive(T o) { return ((UnityEngine::Object *) o)->Alive(); }

template<typename T1, typename T2>
inline bool IsSameUnityObject(T1 o1, T2 o2) {
    auto obj1 = (UnityEngine::Object *) o1;
    auto obj2 = (UnityEngine::Object *) o2;
    return obj1->Same(obj2);
}

}
}
