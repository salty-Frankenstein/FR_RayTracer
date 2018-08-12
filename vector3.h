#pragma once

struct Vector3 {
	Vector3() {}
	Vector3(double _x, double _y, double _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3 Normalize();

	Vector3 operator +(Vector3 k);
	Vector3 operator -(Vector3 k);

	Vector3 operator *(double k);
	Vector3 operator /(double k);

	double dot(Vector3 a);
	Vector3 cross(Vector3 a);

	double x, y, z;
};

Vector3 Vector3::Normalize() {
	Vector3 t;
	double sum = abs(x) + abs(y) + abs(z);
	t.x = x / sum;
	t.y = y / sum;
	t.z = z / sum;
	return t;
}

Vector3 Vector3::operator +(Vector3 k) {
	Vector3 t;
	t.x = x + k.x;
	t.y = y + k.y;
	t.z = z + k.z;
	return t;
}

Vector3 Vector3::operator -(Vector3 k) {
	Vector3 t;
	t.x = x - k.x;
	t.y = y - k.y;
	t.z = z - k.z;
	return t;
}

Vector3 Vector3::operator *(double k) {
	Vector3 t;
	t.x = x * k;
	t.y = y * k;
	t.z = z * k;
	return t;
}

Vector3 Vector3::operator /(double k) {
	Vector3 t;
	t.x = x / k;
	t.y = y / k;
	t.z = z / k;
	return t;
}

double Vector3::dot(Vector3 a) {
	return a.x*x + a.y*y + a.z*z;
}

Vector3 Vector3::cross(Vector3 a) {
	return Vector3(
		y * a.z - z * a.y,
		z * a.x - x * a.z,
		x * a.y - y * a.x
	);
}