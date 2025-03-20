//
// Created by ASUS on 2025/3/12.
//
/************************
 * @Author: Magical1
 * @Time: 2025/3/12 20:00
 * @File: onb.h
 * @Software: CLion
 * @Project: RayTracingInOneWeekend
 * @Description: This class use normal vector to create an orthonormal basis.
 */

#ifndef RAYTRACINGINONEWEEKEND_INCLUDE_ONB_H_
#define RAYTRACINGINONEWEEKEND_INCLUDE_ONB_H_

#include "rtweekend.h"

class onb {
 public:
  explicit onb(const vec3& n) {
    axis[2] = unit_vector(n);
    vec3 a = (std::fabs(axis[2].x()) > 0.9) ? vec3(0, 1, 0) : vec3(1, 0, 0);
    axis[1] = unit_vector(cross(axis[2], a));
    axis[0] = cross(axis[2], axis[1]);
  }

  [[nodiscard]] const vec3& u() const { return axis[0]; }
  [[nodiscard]] const vec3& v() const { return axis[1]; }
  [[nodiscard]] const vec3& w() const { return axis[2]; }

  // Transform a vector from the ONB coordinate system to world space coordinates
  // v[0]: x-component in ONB coordinates (u-direction)
  // v[1]: y-component in ONB coordinates (v-direction)
  // v[2]: z-component in ONB coordinates (w-direction)
  // Returns: The vector transformed into world space by linear combination of basis vectors
  [[nodiscard]] vec3 transform(const vec3& v) const {
      // Transform from basis coordinates to local space.
      return (v[0] * axis[0] + v[1] * axis[1] + v[2] * axis[2]);
  }

 private:
  vec3 axis[3];
};
#endif //RAYTRACINGINONEWEEKEND_INCLUDE_ONB_H_
