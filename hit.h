#pragma once

#include<list>
class HitRecord {
public:
	double t;
	Vector3 p;
	Vector3 normal;
};

class Hitable {
public:
	virtual bool hit(Ray ray, double t_min, double t_max, HitRecord &rec) = 0;
};

class Sphere :public Hitable {
public:
	Vector3 center;
	double radius;
	Sphere(Vector3 cen, double rad) {
		center = cen;
		radius = rad;
	}
	bool hit(Ray ray, double t_min, double t_max, HitRecord &rec);
};

bool Sphere::hit(Ray ray, double t_min, double t_max, HitRecord &rec) {
	auto oc = ray.original - center;
	double a = ray.direction.dot(ray.direction);
	double b = 2.0f*oc.dot(ray.direction);
	double c = oc.dot(oc) - radius * radius;

	double discriminant = b * b - 4 * a*c;
	if (discriminant > 0) {
		double temp = (-b - sqrt(discriminant)) / a * 0.5f;
		if (temp<t_max&&temp>t_min) {
			rec.t = temp;
			rec.p = ray.GetPoint(rec.t);
			rec.normal = (rec.p - center).Normalize();
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a * 0.5f;
		if (temp<t_max&&temp>t_min) {
			rec.t = temp;
			rec.p = ray.GetPoint(rec.t);
			rec.normal = (rec.p - center).Normalize();
			return true;
		}
	}
	return false;
}

class HitableList :Hitable {
public:
	std::list<Hitable*> tlist;
	HitableList() {	}
	bool hit(Ray ray, double t_min, double t_max, HitRecord &rec);
};

bool HitableList::hit(Ray ray, double t_min, double t_max, HitRecord &rec) {
	HitRecord temp;
	bool hitAny = false;
	double closest = t_max;
	for (auto iter = tlist.begin(); iter != tlist.end(); ++iter) {
		if ((**iter).hit(ray, t_min, closest, temp)) {
			hitAny = true;
			closest = temp.t;
			rec = temp;
		}

	}
	return hitAny;
}
