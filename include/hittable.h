//
// Created by ASUS on 2025/2/16.
//

/************************
 * @Author: Magical1
 * @Time: 2025/2/16 20:00
 * @File: hittable.h
 * @Software: CLion
 * @Project: RayTracingInOneWeekend
 * @Description:
 */

#ifndef RAYTRACINGINONEWEEKEND_HITTABLE_H
#define RAYTRACINGINONEWEEKEND_HITTABLE_H

#include "rtweekend.h"

#include "AABB.h"

class material;


class hit_record {
    // The hit_record class stores the point of intersection,
    // the normal at the point of intersection,
    // and the distance from the ray origin to the point of intersection.
public:
    point3  p; // The point of intersection.
    vec3    normal; // The normal at the point of intersection.
    shared_ptr<material> mat; // The material of the object.
    double  t; // The distance from the ray origin to the point of intersection.
    double  u; // The u texture coordinate.
    double  v; // The v texture coordinate;
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
    virtual AABB bounding_box() const = 0;
};

class translate : public hittable {
  // The class represents a translated object.
 public:
  translate(const shared_ptr<hittable>& object, const vec3& offset)
      : object(object), offset(offset)
      {
    bbox = object->bounding_box() + offset;
      }
  bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
    // Move the ray backwards by the offset.
    ray offset_r(r.origin() - offset, r.direction(), r.time());

    // Determine whether an intersection exists along the offset ray(and if so, where).
    if (!object->hit(offset_r, ray_t, rec))
      return false;

    // Move the intersection point forwards by the offset
    rec.p += offset;

    return true;
  }

  AABB bounding_box() const override {
    return bbox;
  }

 private:
  shared_ptr<hittable> object;
  vec3 offset;
  AABB bbox;
};

class rotate_y : public hittable {
  // The class for y-axis rotation.
public:
  rotate_y(const shared_ptr<hittable>&object, double angle) : object(object) {
    auto radians = degrees_to_radians(angle);
    sin_theta = std::sin(radians);
    cos_theta = std::cos(radians);
    bbox = object->bounding_box();

    point3 min(+infinity, +infinity, +infinity);
    point3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; ++i)
      for (int j = 0; j < 2; ++j)
        for (int k = 0; k <2; ++k) {
          auto x = i * bbox.x.max + (1 - i) * bbox.x.min;
          auto y = j * bbox.y.max + (1 - j) * bbox.y.min;
          auto z = k * bbox.z.max + (1 - k) * bbox.z.min;

          auto newx = cos_theta * x + sin_theta * z;
          auto newz = -sin_theta * x + cos_theta * z;

          vec3 tester(newx, y, newz);

          for (int c = 0; c < 3; ++c) {
            min[c] = std::fmin(min[c], tester[c]);
            max[c] = std::fmax(max[c], tester[c]);
          }
        }

    bbox = AABB(min, max);
  }

  bool hit(const ray& r, interval ray_t, hit_record& rec) const override {

    // Transform the ray from world space to object space.

    auto origin = point3(
        (cos_theta * r.origin().x()) - (sin_theta * r.origin().z()),
        r.origin().y(),
        (sin_theta * r.origin().x()) + (cos_theta * r.origin().z())
        );

    auto direction = vec3(
        (cos_theta * r.direction().x()) - (sin_theta * r.direction().z()),
        r.direction().y(),
        (sin_theta * r.direction().x()) + (cos_theta * r.direction().z())
        );

    ray rotated_r(origin, direction, r.time());

    // Determine whether an intersection exists in object space(and if so, where).

    if (!object->hit(rotated_r, ray_t, rec))
      return false;

    // Transform the intersection point and normal vector back to world space.

    rec.p = point3(
        (cos_theta * rec.p.x()) + (sin_theta * rec.p.z()),
        rec.p.y(),
        (-sin_theta * rec.p.x()) + (cos_theta * rec.p.z())
        );

    rec.normal = vec3(
        (cos_theta * rec.normal.x()) + (sin_theta * rec.normal.z()),
        rec.normal.y(),
        (-sin_theta * rec.normal.x()) + (cos_theta * rec.normal.z())
        );

    return true;
  }

  AABB bounding_box() const override {
    return bbox;
  }

 private:
    shared_ptr<hittable> object;
    double sin_theta;
    double cos_theta;
    AABB bbox;
};
#endif //RAYTRACINGINONEWEEKEND_HITTABLE_H
