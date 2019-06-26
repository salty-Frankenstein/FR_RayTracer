#pragma once
#include "hitable.h"
#include<list>

class HitableList :public Hitable {
public:
	HitableList(){}
	HitableList(std::list<Hitable*>& _l) {
		l = _l;
	}
	virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& rec)const;

private:
	std::list<Hitable*> l;
};

bool HitableList::Hit(const Ray& r, float tMin, float tMax, HitRecord& rec)const {
	HitRecord tempRec;
	bool hitAnything = false;
	double closestSoFar = tMax;
	for (auto iter = l.begin(); iter != l.end(); ++iter) {
		if ((**iter).Hit(r, tMin, closestSoFar, tempRec)) {
			hitAnything = true;
			closestSoFar = tempRec.t;
			rec = tempRec;
		}
	}
	return hitAnything;
}
