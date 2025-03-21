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
        get_sphere_uv(outward_normal, rec.u, rec.v);
        rec.mat = mat;

        return true;
    }

    [[nodiscard]] AABB bounding_box() const override {
        return bbox;
    }

    // Calculate the probability density function (PDF) value for a given ray direction
    // relative to a point in space.
    [[nodiscard]] double pdf_value(const point3& origin, const vec3& direction) const override {
        // Create a temporary hit record and check if the ray hits the sphere
        hit_record rec;
        if (!this->hit(ray(origin, direction), interval(0.001, infinity), rec))
            return 0;

        // Calculate the squared distance from the origin to the sphere center
        auto dist_squared = (center.at(0) - origin).length_squared();

        // Calculate the cosine of the maximum angle between the ray and sphere center
        auto cos_theta_max = std::sqrt(1 - radius*radius / dist_squared);

        // Calculate the solid angle subtended by the sphere from the origin point
        auto solid_angle = 2*pi*(1 - cos_theta_max);

        // Return the reciprocal of the solid angle as the PDF value
        return 1 / solid_angle;
    }

    // Generate a random direction vector pointing from the origin towards the sphere
    [[nodiscard]] vec3 random(const point3& origin) const override {
        // Calculate direction from origin to sphere center
        vec3 direction = center.at(0) - origin;
        // Calculate squared distance between origin and sphere center
        auto distance_squared = direction.length_squared();
        // Create orthonormal basis aligned with direction to sphere
        onb uvw(direction);
        // Generate and transform random direction vector weighted by solid angle
        return uvw.transform(random_to_sphere(radius, distance_squared));
    }
private:
    // Data
    ray center;
    double radius;
    shared_ptr<material> mat;
    AABB bbox;

    static void get_sphere_uv(const point3& p, double& u, double& v) {
        // Get the spherical coordinates of a point on the unit sphere.
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from the Y=-1 to Y=+1.
        //      <1 0 0>yields<0.50 0.50>      <-1 0 0>yields<0.00 0.50>
        //      <0 1 0>yields<0.50 1.00>      <0 -1 0>yields<0.50 0.00>
        //      <0 0 1>yields<0.25 0.50>      <0 0 -1>yields<0.75 0.50>

        auto theta = std::acos(-p.y());
        auto phi = std::atan2(-p.z(), p.x()) + pi;

        u = phi / (2*pi);
        v = theta / pi;
    }

    // Helper function to generate a random direction vector towards a sphere
    // radius: The radius of the sphere
    // distance_squared: The squared distance from the sampling point to sphere center
    static vec3 random_to_sphere(double radius, double distance_squared) {
        // Generate two random numbers between 0 and 1
        auto r1 = random_double();
        auto r2 = random_double();

        // Calculate z-coordinate based on sphere radius and distance
        // This ensures proper weighting of random points based on solid angle
        auto z = 1 + r2 * (std::sqrt(1 - radius*radius / distance_squared) - 1);

        // Generate random angle phi in [0, 2π]
        auto phi = 2 * pi * r1;

        // Calculate x and y coordinates using spherical coordinates
        // sqrt(1-z*z) represents the radius of the circle at height z
        auto x = std::cos(phi) * std::sqrt(1 - z*z);
        auto y = std::sin(phi) * std::sqrt(1 - z*z);

        // Return the normalized direction vector
        return {x, y, z};
    }
};
#endif //RAYTRACINGINONEWEEKEND_SPHERE_H
