#pragma once
class Ray {
public:
	Vector3 original, direction, normalDirection;
	Ray(Vector3 o, Vector3 d) {
		original = o;
		direction = d;
		normalDirection = d.Normalize();
	}
	Vector3 GetPoint(double t);

};

Vector3 Ray::GetPoint(double t) {
	return original + direction * t;
}