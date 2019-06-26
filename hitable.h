#pragma once
#include "ray.h"

class Material;

struct HitRecord{
	float t;
	Vec3 p;
	Vec3 normal;
	Material *matPtr;
};

class Hitable {
public:
	virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& rec)const = 0;
};
