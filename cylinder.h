#ifndef CYLINDER_H
#define CYLINDER_H

#include "hittable.h"
#include "vec3.h"
#include "material.h"
#include "interval.h"
#include "stdio.h"

class cylinder : public hittable
{
public:
    cylinder(point3 _center, double _radius, interval _z_interval, shared_ptr<material> _material) : center(_center), radius(_radius), z_interval(_z_interval), mat(_material) {}

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override
    {
        point3 point_a = r.origin();
        vec3 vector_b = r.direction();
        vec3 ac = r.origin() - center;

        auto a = vector_b.x() * vector_b.x() + vector_b.y() * vector_b.y();
        auto half_b = vector_b.x() * ac.x() + vector_b.y() * ac.y();
        auto c = ac.x() * ac.x() + ac.y() * ac.y();
        auto discriminant = half_b * half_b - a * c - radius * radius;
        if (discriminant < 0)
        {
            return false;
        }
        auto sqrtd = sqrt(discriminant);
        auto root = (-half_b - sqrtd) / a;
        if (!(ray_t.surrounds(root) && z_interval.contains(r.at(root).z())))
        {
            root = (-half_b + sqrtd) / a;
            if (!(ray_t.surrounds(root) && z_interval.contains(r.at(root).z())))
            {
                return false;
            }
        }
        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;

        // vec3 oc = r.origin() - center;
        // auto a = r.direction().length_squared();
        // auto half_b = dot(oc, r.direction());
        // auto c = dot(oc, oc) - radius * radius;
        // auto discriminant = half_b * half_b - a * c;
        // if (discriminant < 0)
        // {
        //     return false;
        // }
        // auto sqrtd = sqrt(discriminant);

        // auto root = (-half_b - sqrtd) / a;
        // if (!ray_t.surrounds(root))
        // {
        //     root = (-half_b + sqrtd) / a;
        //     if (!ray_t.surrounds(root))
        //     {
        //         return false;
        //     }
        // }

        // rec.t = root;
        // rec.p = r.at(rec.t);
        // vec3 outward_normal = (rec.p - center) / radius;
        // rec.set_face_normal(r, outward_normal);
        // rec.mat = mat;

        // return true;
    }

private:
    point3 center;
    double radius;
    interval z_interval;
    shared_ptr<material> mat;
};

#endif