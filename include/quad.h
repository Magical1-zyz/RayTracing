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

      area = n.length();

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

  [[nodiscard]] double pdf_value(const point3& origin, const vec3& direction) const override {
    hit_record rec;
    // Try to hit the surface from the given origin along the given direction
    if (!hit(ray(origin, direction), interval(0.001, infinity), rec))
        return 0;  // Return 0 if no hit occurs

    // Calculate the squared distance from origin to hit point
    auto distance_squared = rec.t * rec.t * direction.length_squared();
    // Calculate the absolute cosine of the angle between direction and surface normal
    auto cosine = std::fabs(dot(direction, rec.normal) / direction.length());

    // important sample the light(quad) with respect to the area
    // Return PDF value based on the geometry factor: distance² / (cos(θ) * area)
    return distance_squared / (cosine * area);
  }

  [[nodiscard]] vec3 random(const vec3& origin) const override {
    // Generate a random point on the quad.
    return Q + random_double() * u + random_double() * v - origin;
  }

 protected:
  point3  Q;      // The origin of the quadrilateral surface.
  vec3    u;      // The first axis of the quadrilateral surface.
  vec3    v;      // The second axis of the quadrilateral surface.
  vec3    w;      // w = n/n·n
  vec3    normal; // The normal of the quadrilateral surface.
  double  D;      // Ax + By + Cz = D
  double  area;   // The surface area of the quadrilateral, calculated as the length of the cross product of u and v vectors
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

class tri : public quad {
  // The tri class represents a triangular surface.
 public:
    tri(const point3& o, const vec3& aa, const vec3& ab, shared_ptr<material> m)
        : quad(o, aa, ab, std::move(m))
        {}

    virtual bool is_interior(double a, double b, hit_record& rec) const override {
      if ((a < 0) || (b < 0) || (a + b > 1))
        return false;

      rec.u = a;
      rec.v = b;
      return true;
    }
};

class ellipse : public quad {
  // The ellipse class represents an elliptical surface.
 public:
    ellipse(
        const point3& center, const vec3& side_A, const vec3& side_B, shared_ptr<material> m
        ) : quad(center, side_A, side_B, std::move(m))
        {}  // The ellipse is a quad with a center and two axes.

    virtual void set_bounding_box() override {
      // Compute the bounding box of the ellipse
      bbox = AABB(Q - u - v, Q + u + v);
    }

    virtual bool is_interior(double a, double b, hit_record& rec) const override {
      if ((a * a + b * b) > 1)
        return false;

      rec.u = a/2 + 0.5;
      rec.v = b/2 + 0.5;
      return true;
    }
};

class annulus : public quad {
  // The annulus class represents an annular surface.
 public:
  annulus(
      const point3& center, const vec3& side_A, const vec3& side_B, double _inner,
      shared_ptr<material> m)
      : quad(center, side_A, side_B, std::move(m)), inner(_inner)
      {}

  virtual void set_bounding_box() override {
    // Compute the bounding box of the annulus.
    bbox = AABB(Q - u - v, Q + u + v);
  }

  virtual bool is_interior(double a, double b, hit_record& rec) const override {
    if ((a * a + b * b) > 1 || (a * a + b * b) < inner * inner)
      return false;

    rec.u = a / 2 + 0.5;
    rec.v = b / 2 + 0.5;
    return true;
  }

 private:
  double inner; // The inner radius of the annulus.
};
#endif //RAYTRACINGINONEWEEKEND_INCLUDE_QUAD_H_
