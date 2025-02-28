//
// Created by ASUS on 2025/2/26.
//
/************************
 * @Author: Magical1
 * @Time: 2025/2/26 20:00
 * @File: quad.h
 * @Software: CLion
 * @Project: RayTracingInOneWeekend
 * @Description: The quad class represents a quadrilateral surface.
 */
#ifndef RAYTRACINGINONEWEEKEND_INCLUDE_QUAD_H_
#define RAYTRACINGINONEWEEKEND_INCLUDE_QUAD_H_

#include <utility>

#include "rtweekend.h"

#include "hittable.h"

class quad : public hittable {
  // The quad class represents a quadrilateral surface.
public:
  quad(const point3& Q, const vec3& u, const vec3& v, shared_ptr<material> mat)
      : Q(Q), u(u), v(v), mat_ptr(std::move(mat))
    {
      auto n = cross(u, v);
      normal = unit_vector(n);
      D = dot(Q, normal);
      w = n / dot(n, n);

      set_bounding_box();
    }

  virtual void set_bounding_box() {
    // Compute the bounding box of all four vertices.
    auto bbox_diagonal1 = AABB(Q, Q + u + v);
    auto bbox_diagonal2 = AABB(Q + u, Q + v);
    bbox = AABB(bbox_diagonal1, bbox_diagonal2);
  }

  AABB bounding_box() const override {
    return bbox;
  }

  bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
    auto denom = dot(normal, r.direction());

    // No hit if the ray is parallel to the plane.
    if (std::fabs(denom) < 1e-8)
        return false;

    // Return false if the hit point parameter t is outside the ray interval.
    auto t = (D - dot(normal, r.origin())) / denom;
    if (!ray_t.contains(t))
        return false;

    // Determine if the hit point lies within the planar shape using its plane coordinates.
    auto intersection = r.at(t);
    vec3 planar_hitpt_vector = intersection - Q;
    auto alpha = dot(w, cross(planar_hitpt_vector, v));
    auto beta = dot(w, cross(u, planar_hitpt_vector));

    if (!is_interior(alpha, beta, rec))
      return false;

    // Ray hits the 2D shape; set the hit rest of the hit record and return true.
    rec.t     = t;
    rec.p     = intersection;
    rec.mat   = mat_ptr;
    rec.set_face_normal(r, normal);

    return true;
  }

  virtual bool is_interior(double alpha, double beta, hit_record& rec) const {
    interval unit_interval(0, 1);
    // Given the hit point in plane coordinates, return false if it lies outside the
    // primitive, otherwise set the hit record UV coordinates and return true.

    if (!unit_interval.contains(alpha) || !unit_interval.contains(beta))
      return false;

    rec.u = alpha;
    rec.v = beta;
    return true;
  }

private:
  point3  Q;      // The origin of the quadrilateral surface.
  vec3    u;      // The first axis of the quadrilateral surface.
  vec3    v;      // The second axis of the quadrilateral surface.
  vec3    w;      // w = n/n·n
  vec3    normal; // The normal of the quadrilateral surface.
  double  D;      // Ax + By + Cz = D
  AABB    bbox;   // The bounding box of the quadrilateral surface.
  shared_ptr<material> mat_ptr; // The material of the quadrilateral surface.
};

inline shared_ptr<hittable_list> box(const point3& a, const point3& b, const shared_ptr<material>& mat)
{
  // Returns the 3D box (six sides) that contains the two opposite vertices a and b.

  auto sides = make_shared<hittable_list>();

  // Constructs the two opposite vertices with the minimum and maximum coordinates.
  auto min = point3(std::fmin(a.x(), b.x()), std::fmin(a.y(), b.y()), std::fmin(a.z(), b.z()));
  auto max = point3(std::fmax(a.x(), b.x()), std::fmax(a.y(), b.y()), std::fmax(a.z(), b.z()));

  auto dx = vec3(max.x() - min.x(), 0, 0);
  auto dy = vec3(0, max.y() - min.y(), 0);
  auto dz = vec3(0, 0, max.z() - min.z());

  sides->add(make_shared<quad>(point3(min.x(), min.y(), max.z()), dx,  dy,  mat));  // front
  sides->add(make_shared<quad>(point3(max.x(), min.y(), max.z()), -dz, dy,  mat));  // right
  sides->add(make_shared<quad>(point3(max.x(), min.y(), min.z()), -dx, dy,  mat));  // back
  sides->add(make_shared<quad>(point3(min.x(), min.y(), min.z()), dz,  dy,  mat));  // left
  sides->add(make_shared<quad>(point3(min.x(), max.y(), max.z()), dx,  -dz, mat));  // top
  sides->add(make_shared<quad>(point3(min.x(), min.y(), min.z()), dx,  dz,  mat));  // bottom

  return sides; // Return the six sides of the box.
}
#endif //RAYTRACINGINONEWEEKEND_INCLUDE_QUAD_H_
