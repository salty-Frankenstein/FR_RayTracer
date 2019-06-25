import numpy as np
from abc import ABCMeta, abstractmethod
import sys
import random
import math
import time 

start = time.perf_counter()

class Vec3:
    def __init__(self, e0, e1, e2):
        self.e=np.empty([3],dtype = np.float32)
        self.e[0]=e0
        self.e[1]=e1
        self.e[2]=e2

    def x(self):return self.e[0]
    def y(self):return self.e[1]
    def z(self):return self.e[2]
    def r(self):return self.e[0]
    def g(self):return self.e[1]
    def b(self):return self.e[2]
    
    def __str__(self):return "Vector(%f,%f,%f)"%(self.e[0],self.e[1],self.e[2])

    def __pos__(self):return self
    def __neg__(self):return Vec3(-self.e[0],-self.e[1],-self.e[2])
    def __setitem__(self,i,v):self.e[i]=v
    def __getitem__(self,i):return self.e[i]

    def __add__(self,lhs):return Vec3(self.e[0]+lhs.e[0],self.e[1]+lhs.e[1],self.e[2]+lhs.e[2])
    def __sub__(self,lhs):return Vec3(self.e[0]-lhs.e[0],self.e[1]-lhs.e[1],self.e[2]-lhs.e[2])
    def __rmul__(self,lhs):return Vec3(self.e[0]*lhs,self.e[1]*lhs,self.e[2]*lhs)
    def __mul__(self,lhs):return Vec3(self.e[0]*lhs,self.e[1]*lhs,self.e[2]*lhs)
    def __truediv__(self,lhs):return Vec3(self.e[0]/lhs,self.e[1]/lhs,self.e[2]/lhs)

    def Length(self):
        return (self.e[0] ** 2 + self.e[1] ** 2 + self.e[2] ** 2) ** 0.5

    def SquaredLength(self):
        return self.e[0] ** 2 + self.e[1] ** 2 + self.e[2] ** 2
    
def Dot(a, b):
    return np.vdot(a.e, b.e)

def Mul(a, b):
    return Vec3(a[0]*b[0],a[1]*b[1],a[2]*b[2])

def Cross(a, b):
    return Vec3(
        (a[1] * b[2] - a[2] * b[1]),
        (-(a[0] * b[2] - a[2] * b[0])),
        (a[0] * b[1] - a[1] * b[0])
    )

class Ray:
    def __init__(self, a, b):
        self.A = a
        self.B = b
    
    def Origin(self):return self.A
    def Direction(self):return self.B
    def PointAtParameter(self,t):return self.A + t * self.B

def RandomInUnitDisk():
    p = 2.0 * Vec3(Rand(), Rand(), 0) - Vec3(1, 1, 0)
    while Dot(p, p) >= 1.0:
        p = 2.0 * Vec3(Rand(), Rand(), 0) - Vec3(1, 1, 0)
    return p

class Camera:
    def __init__(self, lookFrom, lookAt, vup, vfov, aspect, aperture, focusDist): #vfov is top to bottom in degrees
        self.lensRadius = aperture / 2
        theta = vfov * math.pi / 180
        halfHeight = math.tan(theta / 2)
        halfWidth = aspect * halfHeight
        self.origin = lookFrom
        self.w = UnitVector(lookFrom - lookAt)
        self.u = UnitVector(Cross(vup, self.w))
        self.v = Cross(self.w, self.u)

        #self.lowerLeftCorner = Vec3(-halfWidth, -halfHeight, -1.0)
        #self.lowerLeftCorner = self.origin - halfWidth * u - halfHeight * v - w
        self.lowerLeftCorner = self.origin - halfWidth * focusDist * self.u - halfHeight * focusDist * self.v - focusDist * self.w
        
        self.horizontal = 2 * halfWidth * focusDist * self.u
        self.vertical = 2 * halfHeight * focusDist * self.v
        #self.horizontal = Vec3(2 * halfWidth, 0.0, 0.0)
        #self.vertical = Vec3(0.0, 2 * halfHeight, 0.0)
    
    def GetRay(self, s, t):
        rd = self.lensRadius * RandomInUnitDisk()
        offset = self.u * rd.x() + self.v * rd.y()
        return Ray(self.origin + offset, self.lowerLeftCorner + s * self.horizontal + t * self.vertical - self.origin - offset)
        #return Ray(self.origin, self.lowerLeftCorner + u*self.horizontal + v*self.vertical - self.origin)

class HitRecord:
    def __init__(self, _t, _p, _n, _m):
        self.t = _t
        self.p = _p
        self.normal = _n
        self.matPtr = _m


class Hitable:
    __metaclass__ = ABCMeta
    def __init__(self):
        pass
    
    @abstractmethod
    def Hit(self, r, tMin, tMax, rec):
        pass


def Rand():
    return random.random()

def RandomInUnitSphere():
    p = 2.0 * Vec3(Rand(), Rand(), Rand()) - Vec3(1, 1, 1)
    while p.SquaredLength() >= 1.0:
        p = 2.0 * Vec3(Rand(), Rand(), Rand()) - Vec3(1, 1, 1)
    return p

class Material:
    __metaclass__ = ABCMeta
    def __init__(self):
        pass
    
    @abstractmethod
    def scatter(self, rIn, rec, attenuation, scattered):
        pass

class Lambertian(Material):
    def __init__(self, a):
        self.albedo = a

    def scatter(self, rIn, rec, attenuation, scattered):
        target = rec.p +rec.normal + RandomInUnitSphere()
        t = Ray(rec.p, target - rec.p)
        scattered.A = t.A
        scattered.B = t.B
        #scattered = Ray(rec.p, target - rec.p)
        attenuation[0] = self.albedo[0]
        attenuation[1] = self.albedo[1]
        attenuation[2] = self.albedo[2]
        #attenuation = self.albedo
        return True

def Reflect(v, n):
    return v - 2 * Dot(v, n) * n

class Metal(Material):
    def __init__(self, a, f):
        self.albedo = a
        if f < 1:
            self.fuzz = f
        else:
            self.fuzz = 1
    
    def scatter(self, rIn, rec, attenuation, scattered):
        reflected = Reflect(UnitVector(rIn.Direction()), rec.normal)
        t = Ray(rec.p, reflected + self.fuzz * RandomInUnitSphere())
        scattered.A = t.A
        scattered.B = t.B
        #scattered = Ray(rec.p, reflected)
        attenuation[0] = self.albedo[0]
        attenuation[1] = self.albedo[1]
        attenuation[2] = self.albedo[2]
        #attenuation = self.albedo
        return (Dot(scattered.Direction(), rec.normal) > 0)

def Refract(v, n, niOverNt, refracted):
    uv = UnitVector(v)
    dt = Dot(uv, n)
    discriminant = 1.0 - (niOverNt ** 2) * (1 - dt ** 2)
    if discriminant > 0:
        t = niOverNt * (uv - n * dt) - n * (discriminant ** 0.5)
        refracted[0] = t[0]
        refracted[1] = t[1]
        refracted[2] = t[2]
        return True
    else:
        return False

def Schlick(cosine, refIdx):
    r0 = (1 - refIdx) / (1 + refIdx)
    r0 = r0 **2
    return r0 + (1 - r0) * (1 - cosine) ** 5

class Dielectric(Material):
    def __init__(self, ri):
        self.refIdx = ri

    def scatter(self, rIn, rec, attenuation, scattered):
        outwardNormal = Vec3(0,0,0)
        reflected = Reflect(rIn.Direction(), rec.normal)
        niOverNt = 0.0
        attenuation = Vec3(1.0, 1.0, 1.0)
        refracted = Vec3(1.0, 1.0, 1.0)
        if Dot(rIn.Direction(), rec.normal) > 0:
            outwardNormal = -rec.normal
            niOverNt = self.refIdx
            cosine = self.refIdx * Dot(rIn.Direction(), rec.normal) / rIn.Direction().Length()
        else:
            outwardNormal = rec.normal
            niOverNt = 1.0 / self.refIdx
            cosine = -Dot(rIn.Direction(), rec.normal) / rIn.Direction().Length()
        if Refract(rIn.Direction(), outwardNormal, niOverNt, refracted):
            reflectProb = Schlick(cosine, self.refIdx)
            #t = Ray(rec.p, refracted)
            #scattered.A = t.A
            #scattered.B = t.B
            ##scattered = Ray(rec.p, refracted)
        else:
            t = Ray(rec.p, reflected)
            scattered.A = t.A
            scattered.B = t.B
            reflectProb = 1.0
            ##scattered = Ray(rec.p, reflected)
        if Rand() < reflectProb:
            t = Ray(rec.p, reflected)
            scattered.A = t.A
            scattered.B = t.B
            ##scattered = Ray(rec.p, reflected)
        else:
            t = Ray(rec.p, refracted)
            scattered.A = t.A
            scattered.B = t.B
            ##scattered = Ray(rec.p, refracted)
        return True


class Sphere(Hitable):
    def __init__(self, cen, r, m):
        self.center = cen
        self.radius = r
        self.matPtr = m

    def Hit(self, r, tMin, tMax, rec):
        oc = r.Origin() - self.center
        a = Dot(r.Direction(), r.Direction())
        b = Dot(oc, r.Direction())
        c = Dot(oc, oc) - self.radius ** 2
        discriminant = b ** 2 - a * c
        if discriminant > 0:
            temp = (-b - (b ** 2 - a * c) ** 0.5) / a
            if temp < tMax and temp > tMin:
                rec.t = temp
                rec.p = r.PointAtParameter(rec.t)
                rec.normal = (rec.p - self.center) / self.radius
                rec.matPtr = self.matPtr
                return True
            temp = (-b + (b ** 2 - a * c) ** 0.5) / a
            if temp < tMax and temp > tMin:
                rec.t = temp
                rec.p = r.PointAtParameter(rec.t)
                rec.normal = (rec.p - self.center) / self.radius
                rec.matPtr = self.matPtr
                return True
        else:
            return False


class HitableList(Hitable):
    def __init__(self,l):
        self.list = l
    
    def Hit(self, r, tMin, tMax, rec):
        tempRecord = HitRecord(1, Vec3(1,1,1), Vec3(1,1,1),Metal(Vec3(0,0,0),0))
        hitAnything = False
        closest = tMax
        for h in self.list:
            if h.Hit(r, tMin, closest, tempRecord):
                hitAnything = True
                closest = tempRecord.t 
                rec.p = tempRecord.p
                rec.t = tempRecord.t
                rec.normal = tempRecord.normal
                rec.matPtr =tempRecord.matPtr
                #rec = tempRecord //wrong because it's reference
        return hitAnything


def UnitVector(t):
    d = (t[0] ** 2+t[1] ** 2+t[2] ** 2) ** 0.5
    return t * (1/d)

def HitSphere(center, radius, r):
    oc = r.Origin() - center
    a = Dot(r.Direction(), r.Direction())
    b = 2.0 * Dot(oc, r.Direction())
    c = Dot(oc, oc) - radius ** 2
    discriminant = b ** 2 - 4 * a * c
    if discriminant < 0:
        return -1.0
    else:
        return (-b - discriminant ** 0.5) / (2.0 * a)


def Color(r, world, depth):
    rec = HitRecord(0, Vec3(0,0,0), Vec3(0,0,0),Metal(Vec3(0,0,0), 0))
    if world.Hit(r, 0.01, sys.float_info.max, rec):
        scattered = Ray(Vec3(-1,-1,-1), Vec3(-1,-1,-1))
        attenuation = Vec3(1,1,1)
        if depth < 50 and rec.matPtr.scatter(r, rec, attenuation, scattered):
            return Mul(attenuation, Color(scattered, world, depth+1))
            #return attenuation * Color(scattered, world, depth+1)
        else:
            return Vec3(0, 0, 0)
    else:
        unitDirection = UnitVector(r.Direction())
        t = 0.5 * (unitDirection.y() + 1.0)
        return (1.0 - t) * Vec3(1.0, 1.0 ,1.0) + t * Vec3(0.5, 0.7, 1.0)

def RandomScene():
    n = 500
    list = [Sphere(Vec3(0, -1000, 0), 1000, Lambertian(Vec3(0.5, 0.5, 0.5)))]
    
    for a in range(-11,11):
        for b in range(-11,11):
            chooseMat = Rand()
            center = Vec3(a + 0.9 * Rand(), 0.2, b + 0.9 * Rand())
            if (center - Vec3(4, 0.2, 0)).Length() > 0.9:
                if chooseMat < 0.8:
                    list.append(Sphere(center, 0.2, Lambertian(Vec3(Rand()*Rand(),Rand()*Rand(),Rand()*Rand()))))
                elif chooseMat < 0.95:
                    list.append(Sphere(center, 0.2, 
                    Metal(Vec3(0.5*(1+Rand()), 0.5 * (1+Rand()), 0.5 *(1+Rand())), 0.5 * Rand())))
                else:
                    list.append(Sphere(center, 0.2, Dielectric(1.5)))
    
    list.append(Sphere(Vec3(0, 1, 0), 1.0, Dielectric(1.5)))
    list.append(Sphere(Vec3(-4, 1, 0), 1.0, Lambertian(Vec3(0.4, 0.2, 0.1))))
    list.append(Sphere(Vec3(4, 1, 0), 1.0, Metal(Vec3(0.7, 0.6, 0.5), 0.0)))
    return list

#MAIN#

f=open("outGLS3.ppm","w")

nx = 200
ny = 100
ns = 15
f.write("P3\n%d %d\n255\n"%(nx, ny))

''''
R = math.cos(math.pi / 4)
list = [Sphere(Vec3(-R, 0, -1), R, Lambertian(Vec3(0, 0, 1))),
        Sphere(Vec3(R, 0, -1), R, Lambertian(Vec3(1, 0, 0)))]

'''

'''
list = [Sphere(Vec3(0, 0, -1), 0.5, Lambertian(Vec3(0.1, 0.2, 0.5))),
        Sphere(Vec3(0, -100.5, -1),100, Metal(Vec3(0.8, 0.8, 0.0), 0)),
        Sphere(Vec3(1, 0, -1), 0.5, Metal(Vec3(0.8, 0.6, 0.2), 0)),
        Sphere(Vec3(-1, 0, -1), 0.5, Dielectric(1.5)),
        Sphere(Vec3(-1, 0, -1), -0.45, Dielectric(1.5))
        ]
'''
list = RandomScene()

world = HitableList(list)
#cam = Camera(90, float(nx)/float(ny))
#cam = Camera(Vec3(-2, 2, 1), Vec3(0, 0, -1),Vec3(0, 1, 0), 25, float(nx)/float(ny))
lookFrom = Vec3(13, 2, 3)
lookAt = Vec3(0, 0, 0)
#distToFocus = (lookFrom - lookAt).Length()
distToFocus = 10
aperture = 0.1
#aperture = 2.0
cam = Camera(lookFrom, lookAt, Vec3(0, 1, 0), 20, float(nx)/float(ny), aperture, distToFocus)

for j in range(ny-1, -1, -1):
    for i in range(0, nx):
        if i % 30 == 0 and j % 2 == 0:
            print("%.2f%% completed"% (float((i+(ny-j-1)*nx))/(nx*ny)*100) )
        col = Vec3(0, 0, 0)
        for s in range(0, ns):
            u = float(i + Rand()) / float(nx)
            v = float(j + Rand()) / float(ny) 
            r = cam.GetRay(u, v)
            p = r.PointAtParameter(2.0)
            col += Color(r, world, 0)
        col /= float(ns)
        col = Vec3(col[0] ** 0.5, col[1] ** 0.5, col[2] ** 0.5)
        ir = int(255.99 * col[0])
        ig = int(255.99 * col[1])
        ib = int(255.99 * col[2])
        f.write("%d %d %d\n"%(ir, ig, ib))

f.close()

dur = time.perf_counter() - start
print("time spent:%.2fs"%dur)
