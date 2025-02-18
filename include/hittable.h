//
// Created by ASUS on 2025/2/16.
//

#ifndef RAYTRACINGINONEWEEKEND_HITTABLE_H
#define RAYTRACINGINONEWEEKEND_HITTABLE_H

#include "rtweekend.h"

class material;


class hit_record {
    // The hit_record class stores the point of intersection,
    // the normal at the point of intersection,
    // and the distance from the ray origin to the point of intersection.
public:
    point3  p;
    vec3    normal;
    shared_ptr<material> mat;
    double  t;
    bool    front_face;

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter outward_normal is assumed to have unit length.

        // If the ray is outside the object, the normal points outward.
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
    // The hittable class is an abstract base class for objects
    // that can be hit by rays.
public:
    virtual  ~hittable() = default;

    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};
#endif //RAYTRACINGINONEWEEKEND_HITTABLE_H
