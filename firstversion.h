#pragma once
#include<mypic.h>
#include<vector3.h>
#include<ray.h>
//background
inline pixel Getcolor(Ray ray) {
	double t = 0.5f*(ray.normalDirection.y + 1.0f);
	return pixel(1, 1, 1) * (1 - t) + pixel(0.5f, 0.7f, 1.0f) * t;
}

inline void CreateColor(mypic *p) {
	Vector3 lowLeftCorner(-2, -1, -1);
	Vector3 horizontal(4, 0, 0);
	Vector3 vertical(0, 2, 0);
	Vector3 original(0, 0, 0);
	for (int j = 0; j<p->GHei(); j++)
		for (int i = 0; i<p->GLen(); i++) {
			Ray r(original,
				lowLeftCorner + (horizontal * (p->GLen() - i) / (double)p->GLen()) + vertical * j / (double)p->GHei());
			pixel t = Getcolor(r);
			p->putpixel(j, i, t);
		}
}


//create Sphere
inline bool isHitSphere(Vector3 center, double radius, Ray ray) {
	Vector3 oc = ray.original - center;
	double a = ray.direction.dot(ray.direction);
	double b = 2.0f*oc.dot(ray.direction);
	double c = oc.dot(oc) - radius * radius;

	double discriminant = b * b - 4 * a*c;
	return (discriminant > 0);
}

inline pixel GetColorSphere(Ray ray) {
	Vector3 tv(0, 0, -1);
	if (isHitSphere(tv, 0.5f, ray)) {
		pixel tp(1, 0, 0);
		return tp;
	}
	double t = 0.5f*(ray.normalDirection.y + 1.0f);
	return pixel(1, 1, 1) * (1 - t) + pixel(0.5f, 0.7f, 1.0f) * t;
}

void CreateColorSphere(mypic *p) {
	Vector3 lowLeftCorner(-2, -1, -1);
	Vector3 horizontal(4, 0, 0);
	Vector3 vertical(0, 2, 0);
	Vector3 original(0, 0, 0);
	for (int j = 0; j<p->GHei(); j++)
		for (int i = 0; i<p->GLen(); i++) {
			Ray r(original,
				lowLeftCorner + (horizontal * (p->GLen() - i) / (double)p->GLen()) + vertical * j / (double)p->GHei());
			pixel t = GetColorSphere(r);
			p->putpixel(j, i, t);
		}
}

//Sphere Normal
inline double HitSphereNormal(Vector3 center, double radius, Ray ray) {
	Vector3 oc = ray.original - center;
	double a = ray.direction.dot(ray.direction);
	double b = 2.0f*oc.dot(ray.direction);
	double c = oc.dot(oc) - radius * radius;

	double discriminant = b * b - 4 * a*c;
	if (discriminant < 0)
		return -1;
	else
		return (-b - sqrt(discriminant) / (2.0f*a));
}

inline pixel GetColorNormal(Ray ray) {
	Vector3 tv(0, 0, -1);
	double tt = HitSphereNormal(tv, 0.5f, ray);
	if (tt>0) {
		Vector3 normal = ray.GetPoint(tt) - tv;
		normal = normal.Normalize();
		pixel tp(0.5f*(normal.x + 1), 0.5f*(normal.y + 1), 0.5f*(normal.z + 1));
		return tp;
	}
	double t = 0.5f*(ray.normalDirection.y + 1.0f);
	return pixel(1, 1, 1) * (1 - t) + pixel(0.5f, 0.7f, 1.0f) * t;
}

void CreateColorNormal(mypic *p) {
	Vector3 lowLeftCorner(-2, -1, -1);
	Vector3 horizontal(4, 0, 0);
	Vector3 vertical(0, 2, 0);
	Vector3 original(0, 0, 0);
	for (int j = 0; j<p->GHei(); j++)
		for (int i = 0; i<p->GLen(); i++) {
			Ray r(original,
				lowLeftCorner + (horizontal * (p->GLen() - i) / (double)p->GLen()) + vertical * j / (double)p->GHei());
			pixel t = GetColorNormal(r);
			p->putpixel(j, i, t);
		}
}

