#pragma once
#include <cmath>


class Vec3 {
public:
	Vec3(){}
	Vec3(float e0, float e1, float e2) {
		e[0] = e0;	e[1] = e1;	e[2] = e2;
	}
	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }
	inline float z() const { return e[2]; }
	inline float r() const { return e[0]; }
	inline float g() const { return e[1]; }
	inline float b() const { return e[2]; }

	inline const Vec3& operator + () const { return *this; }
	inline const Vec3 operator - () const { return Vec3(-e[0], -e[1], -e[2]); }
	inline float operator [] (int i) const { return e[i]; }
	inline float& operator [] (int i) { return e[i]; }

	inline Vec3& operator += (const Vec3 &v2);
	inline Vec3& operator -= (const Vec3 &v2);
	inline Vec3& operator *= (const Vec3 &v2);
	inline Vec3& operator /= (const Vec3 &v2);
	inline Vec3& operator *= (const float t);
	inline Vec3& operator /= (const float t);

	inline float Length() const {
		return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	}
	inline float SquaredLength() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}


private:
	float e[3];
};

inline Vec3 operator+(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
}

inline Vec3 operator-(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
}

inline Vec3 operator*(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z());
}

inline Vec3 operator/(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1.x() / v2.x(), v1.y() / v2.y(), v1.z() / v2.z());
}

inline Vec3 operator*(float t, const Vec3 &v) {
	return Vec3(t * v.x(), t * v.y(), t * v.z());
}

inline Vec3 operator*(const Vec3 &v, float t) {
	return Vec3(t * v.x(), t * v.y(), t * v.z());
}

inline Vec3 operator/(const Vec3 &v, float t) {
	return Vec3(v.x() / t, v.y() / t, v.z() / t);
}

inline float Dot(const Vec3 &v1, const Vec3 &v2) {
	return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

inline Vec3 Cross(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(
		v1.y()*v2.z() - v1.z()*v2.y(),
		-(v1.x()*v2.z() - v1.z()*v2.x()),
		v1.x()*v2.y() - v1.y()*v2.x()
	);
}


inline Vec3& Vec3::operator += (const Vec3 &v) {
	e[0] += v.x();
	e[1] += v.y();
	e[2] += v.z();
	return *this;
}

inline Vec3& Vec3::operator -= (const Vec3 &v){
	e[0] -= v.x();
	e[1] -= v.y();
	e[2] -= v.z();
	return *this;
}

inline Vec3& Vec3::operator *= (const Vec3 &v){
	e[0] *= v.x();
	e[1] *= v.y();
	e[2] *= v.z();
	return *this;
}

inline Vec3& Vec3::operator /= (const Vec3 &v){
	e[0] /= v.x();
	e[1] /= v.y();
	e[2] /= v.z();
	return *this;
}

inline Vec3& Vec3::operator *= (const float t){
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline Vec3& Vec3::operator /= (const float t) {
	float k = 1.0 / t;
	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
	return *this;
}

inline Vec3 UnitVector(const Vec3 &v) {
	return v / v.Length();
}