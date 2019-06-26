#pragma once
#include "hitable.h"

class Sphere :public Hitable {
public:
	Sphere(){}
	Sphere(Vec3 cen, float r, Material* m) :center(cen), radius(r), matPtr(m) {};
	virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& rec)const;

private:
	Vec3 center;
	float radius;
	Material *matPtr;
};

bool Sphere::Hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
	Vec3 oc = r.Origin() - center;
	float a = Dot(r.Direction(), r.Direction());
	float b = Dot(oc, r.Direction());
	float c = Dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(b*b - a * c)) / a;
		if (temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.p = r.PointAtParameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.matPtr = matPtr;
			return true;
		}
		temp = (-b + sqrt(b*b - a * c)) / a;
		if (temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.p = r.PointAtParameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.matPtr = matPtr;
			return true;
		}
	}
	return false;
}

