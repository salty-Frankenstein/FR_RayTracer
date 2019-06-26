#pragma once

#include "drand48.h"
struct hit_record;

#include "ray.h"
#include "hitable.h"

class Material {
public:
	virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered)const = 0;
};

Vec3 RandomInUnitSphere() {
	Vec3 p;
	do {
		p = 2.0*Vec3(drand48(), drand48(), drand48()) - Vec3(1, 1, 1);
	} while (p.SquaredLength() >= 1.0);
	return p;
}

class Lambertian :public Material {
public:
	Lambertian(const Vec3& a):albedo(a){}
	virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered)const {
		Vec3 target = rec.p + rec.normal + RandomInUnitSphere();
		scattered = Ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}

private:
	Vec3 albedo;
};

Vec3 Reflect(const Vec3&v, const Vec3& n) {
	return v - 2 * Dot(v, n)*n;
}

class Metal :public Material {
public:
	Metal(const Vec3& a,float f):albedo(a){
		if (f < 1) fuzz = f;
		else fuzz = 1;
	}
	virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered)const {
		Vec3 reflected = Reflect(UnitVector(rIn.Direction()), rec.normal);
		scattered = Ray(rec.p, reflected+fuzz*RandomInUnitSphere());
		attenuation = albedo;
		return (Dot(scattered.Direction(), rec.normal) > 0);
	}

private:
	Vec3 albedo;
	float fuzz;
};

bool Refract(const Vec3& v, const Vec3& n, float niOverNt, Vec3& refracted) {
	Vec3 uv = UnitVector(v);
	float dt = Dot(uv, n);
	float discriminant = 1.0 - niOverNt * niOverNt*(1 - dt * dt);
	if (discriminant > 0) {
		refracted = niOverNt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else return false;
}

float Schlick(float cosine, float refIdx) {
	float r0 = (1 - refIdx) / (1 + refIdx);
	r0 = r0 * r0;
	return r0 + (1 - r0)*pow((1 - cosine), 5);
}

class Dielectric :public Material {
public:
	Dielectric(float ri):refIdx(ri){}
	virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered)const {
		Vec3 outwardNormal;
		Vec3 reflected = Reflect(rIn.Direction(), rec.normal);
		float niOverNt;
		attenuation = Vec3(1.0, 1.0, 1.0);
		Vec3 refracted;
		float reflectProb;
		float cosine;
		if (Dot(rIn.Direction(), rec.normal) > 0) {
			outwardNormal = -rec.normal;
			niOverNt = refIdx;
			cosine = refIdx * Dot(rIn.Direction(), rec.normal) / rIn.Direction().Length();
		}
		else {
			outwardNormal = rec.normal;
			niOverNt = 1.0 / refIdx;
			cosine = -Dot(rIn.Direction(), rec.normal) / rIn.Direction().Length();
		}
		if (Refract(rIn.Direction(), outwardNormal, niOverNt, refracted)) {
			//scattered = Ray(rec.p, refracted);
			reflectProb = Schlick(cosine, refIdx);
		}
		else{
			scattered = Ray(rec.p, reflected);
			reflectProb = 1.0;
		}
		if (drand48() < reflectProb) {
			scattered = Ray(rec.p, reflected);
		}
		else {
			scattered = Ray(rec.p, refracted);
		}
		return true;
	}


private:
	float refIdx;
};