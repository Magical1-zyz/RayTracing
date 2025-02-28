//
// Created by ASUS on 2025/2/28.
//
/************************
 * @Author: Magical1
 * @Time: 2025/2/28 20:00
 * @File: constant_medium.h
 * @Software: CLion
 * @Project: RayTracingInOneWeekend
 * @Description:
 */

#ifndef RAYTRACINGINONEWEEKEND_INCLUDE_CONSTANT_MEDIUM_H_
#define RAYTRACINGINONEWEEKEND_INCLUDE_CONSTANT_MEDIUM_H_

#include <utility>

#include "rtweekend.h"

#include "hittable.h"
#include "material.h"
#include "texture.h"

class constant_medium : public hittable {
  // The constant_medium class represents a volume of constant density.
 public:
  constant_medium(shared_ptr<hittable> boundary, double density, const shared_ptr<texture>& tex)
      : boundary(std::move(boundary)), neg_inv_density(-1/density),
        phase_function(make_shared<isotropic>(tex)) {}

  constant_medium(shared_ptr<hittable> boundary, double density, const color& albedo)
      : boundary(std::move(boundary)), neg_inv_density(-1/density),
        phase_function(make_shared<isotropic>(albedo)) {}

  bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
    // rec1: hit record for the first intersection with the boundary.
    // rec2: hit record for the second intersection with the boundary.
    hit_record rec1, rec2;

    if (!boundary->hit(r, interval::universe, rec1))
      return false;

    if (!boundary->hit(r, interval(rec1.t + 0.0001, infinity), rec2))
      return false;

    if (rec1.t < ray_t.min) rec1.t = ray_t.min;
    if (rec2.t > ray_t.max) rec2.t = ray_t.max;

    if (rec1.t >= rec2.t)
      return false;

    if (rec1.t < 0)
      rec1.t = 0;

    // Calculate the distance the ray travels through the volume.
    const auto ray_length = r.direction().length();
    const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    // Generating random scattering distances using an exponential distribution.
    // Using an exponential distribution to model the probability of light scattering in a medium.
    const auto hit_distance = neg_inv_density * std::log(random_double());

    // Determine whether scattering occurs within the medium.
    if (hit_distance > distance_inside_boundary)
      return false;

    rec.t = rec1.t + hit_distance / ray_length;
    rec.p = r.at(rec.t);

    rec.normal = vec3(1, 0, 0);  // Arbitrary
    rec.front_face = true;        // also arbitrary
    rec.mat = phase_function;

    return true;
  }

  AABB bounding_box() const override {
    return boundary->bounding_box();
  }

 private:
  shared_ptr<hittable> boundary; // The boundary of the volume.
  double neg_inv_density; // The negative inverse density of the volume.
  shared_ptr<material> phase_function; // The phase function of the volume.
};
#endif //RAYTRACINGINONEWEEKEND_INCLUDE_CONSTANT_MEDIUM_H_
