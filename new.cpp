// new.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include<string>
#include "sphere.h"
#include "hitablelist.h"
#include "camera.h"
#include "material.h"
#include "float.h"
#include "drand48.h"

Vec3 Color(const Ray& r, Hitable *world, int depth) {
	HitRecord rec;
	if (world->Hit(r, 0.001, FLT_MAX, rec)) {
		Ray scattered;
		Vec3 attenuation;
		if (depth < 50 && rec.matPtr->Scatter(r, rec, attenuation, scattered)) {
			return attenuation * Color(scattered, world, depth + 1);
		}
		else {
			return Vec3(0, 0, 0);
		}
	}
	else {
		Vec3 unitDirection = UnitVector(r.Direction());
		float t = 0.5 * (unitDirection.y() + 1.0);
		return (1.0 - t)*Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
	}
}

Hitable* RandomScene() {
	int n = 500;
	std::list<Hitable*> *l =new std::list<Hitable*>;
	l->push_back(new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(Vec3(0.5, 0.5, 0.5))));
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float chooseMat = drand48();
			Vec3 center(a + 0.9*drand48(), 0.2, b + 0.9*drand48());
			if ((center - Vec3(4, 0.2, 0)).Length() > 0.9) {
				if (chooseMat < 0.8)
					l->push_back(
						new Sphere(center, 0.2, new Lambertian(Vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())))
					);
				else if(chooseMat<0.95)
					l->push_back(
						new Sphere(center, 0.2, 
							new Metal(Vec3(0.5*(1+drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48())),0.5*drand48())
						)
					);
				else l->push_back(new Sphere(center, 0.2, new Dielectric(1.5)));
			}
		}
	}
	l->push_back(new Sphere(Vec3(0, 1, 0), 1.0, new Dielectric(1.5)));
	l->push_back(new Sphere(Vec3(-4, 1, 0), 1.0, new Lambertian(Vec3(0.4, 0.2, 0.1))));
	l->push_back(new Sphere(Vec3(4, 1, 0), 1.0, new Metal(Vec3(0.7, 0.6, 0.5), 0.0)));
	Hitable* t = new HitableList(*l);
	return t;
}

int main()
{
	int nx = 800, ny = 400, ns = 50;
	FILE *fp;
	std::string name = "大图.ppm";
	fopen_s(&fp, name.c_str(), "wb");
	fprintf_s(fp, "P3\n%d %d\n255\n", nx, ny);

	//std::list<Hitable*> l;

	/*
	float R = cos(PI / 4);
	l.push_back(new Sphere(Vec3(-R, 0, -1), R, new Lambertian(Vec3(0, 0, 1))));
	l.push_back(new Sphere(Vec3(R, 0, -1), R, new Lambertian(Vec3(1, 0, 0))));

	
	l.push_back(new Sphere(Vec3(0, 0, -1), 0.5, new Lambertian(Vec3(0.1, 0.2, 0.5))));
	l.push_back(new Sphere(Vec3(0, -100.5, -1), 100, new Lambertian(Vec3(0.8, 0.8, 0.0))));
	l.push_back(new Sphere(Vec3(1, 0, -1), 0.5, new Metal(Vec3(0.8, 0.6, 0.2),0.3)));
	l.push_back(new Sphere(Vec3(-1, 0, -1), 0.5, new Dielectric(1.5)));
	l.push_back(new Sphere(Vec3(-1, 0, -1), -0.45, new Dielectric(1.5)));
	*/

	//Hitable *world=new HitableList(l);
	Hitable *world = RandomScene();

	Vec3 lookFrom(13, 2, 3);
	Vec3 lookAt(0, 0, 0);
	float distToFocus = 10.0;
	float aperture = 0.1;
	Camera cam(lookFrom,lookAt,Vec3(0,1,0) ,20,float(nx)/float(ny),aperture,distToFocus);

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			if(i%10==0)
				printf("%f\n", ((ny - j - 1)*nx + i) / float(nx*ny)*100);
			Vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				float u = float(i + drand48()) / float(nx);
				float v = float(j + drand48()) / float(ny);
				Ray r = cam.GetRay(u, v);
				//Vec3 p = r.PointAtParameter(2.0)
				col += Color(r, world,0);

			}
			col /= float(ns);
			col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			fprintf_s(fp, "%d %d %d\n", ir, ig, ib);
		}
	}
	fclose(fp);

    return 0;
}

