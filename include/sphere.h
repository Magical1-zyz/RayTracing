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

#include "rtweekend.h"

#include "hittable.h"

#include <utility>

class sphere : public hittable {
public:
    // Constructors
    // Stationary Sphere
    sphere(const point3& static_center, double radius, shared_ptr<material> mat)
        : center(static_center, vec3(0,0,0)),
        radius(std::fmax(0, radius)), mat(std::move(mat))
    {
        auto rvec = vec3(radius, radius, radius);
        bbox = AABB(static_center - rvec, static_center + rvec);
    };
    // Moving Sphere
    sphere(const point3& center1, const point3& center2, double radius,
           shared_ptr<material> mat)
        : center(center1, center2 - center1),
        radius(std::fmax(0, radius)), mat(std::move(mat))
    {
        auto rvec = vec3(radius, radius, radius);
        AABB box1(center.at(0) - rvec, center.at(0) + rvec);
        AABB box2(center.at(1) - rvec, center.at(1) + rvec);
        bbox = AABB(box1, box2);
    };


    // The hit_sphere function takes a center and radius for a sphere, and a ray.
    bool hit(const ray&r, interval ray_t, hit_record& rec) const override {
        point3 current_center = center.at(r.time());
        vec3 oc = current_center - r.origin();
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
        vec3 outward_normal = (rec.p - current_center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

    AABB bounding_box() const override {
        return bbox;
    }
private:
    // Data
    ray center;
    double radius;
    shared_ptr<material> mat;
    AABB bbox;
};
#endif //RAYTRACINGINONEWEEKEND_SPHERE_H
