#pragma once
#include "ray.h"
#include "drand48.h"
#define PI 3.1415926535897932

Vec3 RandomInUnitDisk() {
	Vec3 p;
	do {
		p = 2.0*Vec3(drand48(), drand48(), 0) - Vec3(1, 1, 0);
	} while (Dot(p, p) >= 1.0);
	return p;
}

class Camera {
public:
	Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 vup, float vfov, float aspect, float aperture, float focusDist) {
		lensRadius = aperture / 2;
		float theta = vfov * PI / 180;
		float halfHeight = tan(theta / 2);
		float halfWidth = aspect * halfHeight;
		origin = lookFrom;
		w = UnitVector(lookFrom - lookAt);
		u = UnitVector(Cross(vup, w));
		v = Cross(w, u);

		lowerLeftCorner = origin - halfWidth * focusDist*u - halfHeight * focusDist*v - focusDist * w;
		horizontal = 2 * halfWidth*focusDist*u;
		vertical = 2 * halfHeight*focusDist*v;

	}
	Ray GetRay(float s, float t) {
		Vec3 rd = lensRadius * RandomInUnitDisk();
		Vec3 offset = u * rd.x() + v * rd.y();
		return Ray(origin+offset, lowerLeftCorner + s * horizontal + t * vertical - origin-offset);
	}

private:
	Vec3 lowerLeftCorner;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 origin;
	Vec3 u, v, w;
	float lensRadius;
};
