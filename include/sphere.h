//
// Created by ASUS on 2025/2/16.
//

/************************
 * @Author: Magical1
 * @Time: 2025/2/16 20:00
 * @File: sphere.h
 * @Software: CLion
 * @Project: RayTracingInOneWeekend
 * @Description:
 */

#ifndef RAYTRACINGINONEWEEKEND_SPHERE_H
#define RAYTRACINGINONEWEEKEND_SPHERE_H

#include <utility>

#include "hittable.h"
#include "rtweekend.h"

class sphere : public hittable {
public:
    // Constructors
    sphere(const point3& center, double radius, shared_ptr<material> mat)
        : center(center), radius(radius), mat(std::move(mat)) {};

    // The hit_sphere function takes a center and radius for a sphere, and a ray.
    bool hit(const ray&r, interval ray_t, hit_record& rec) const override {
        vec3 oc = center - r.origin();
        auto a = r.direction().length_squared();
        // h = -b/2 = dot(oc, r.direction())
        auto h = dot(oc, r.direction());
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = h * h - a * c;
        // If the discriminant is negative, the ray missed the sphere.
        if (discriminant < 0)
            return false;

        // Otherwise, find the nearest root that lies in the acceptable range.
        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        // Record the hit information.
        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

private:
    // Data
    point3 center;
    double radius;
    shared_ptr<material> mat;
};
#endif //RAYTRACINGINONEWEEKEND_SPHERE_H
