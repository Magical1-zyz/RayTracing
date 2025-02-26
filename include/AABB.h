//
// Created by ASUS on 2025/2/21.
//
/************************
 * @Author: Magical1
 * @Time: 2025/2/21 20:00
 * @File: AABB.h
 * @Software: CLion
 * @Project: RayTracingInOneWeekend
 * @Description:
 */

#ifndef RAYTRACINGINONEWEEKEND_AABB_H
#define RAYTRACINGINONEWEEKEND_AABB_H

#include "rtweekend.h"

class AABB {
public:
    interval x, y, z;

    AABB() = default; // The default AABB is empty, since intervals are empty by default.

    AABB(const interval& x, const interval& y, const interval& z)
        : x(x), y(y), z(z)
    {
      pad_to_minimums();
    }

    AABB(const point3& a, const point3& b) {
        //Treat the two points a and b as extrema for the bounding box,
        //so we don't require a particular minimum or maximum coordinate order.

        x = (a[0] <= b[0]) ? interval(a[0], b[0]) : interval(b[0], a[0]);
        y = (a[1] <= b[1]) ? interval(a[1], b[1]) : interval(b[1], a[1]);
        z = (a[2] <= b[2]) ? interval(a[2], b[2]) : interval(b[2], a[2]);

        pad_to_minimums();
    }

    AABB(const AABB& box1, const AABB& box2) {
        // Create the AABB tightly enclosing the two input AABBs.
        x = interval(box1.x, box2.x);
        y = interval(box1.y, box2.y);
        z = interval(box1.z, box2.z);
    }

    const interval& axis_interval(int n) const {
        // Return the interval for the given axis.
        if (n == 1) return y;
        if (n == 2) return z;
        return x;
    }

    bool hit(const ray& r, interval ray_t) const {
        // Check for intersection with the AABB.
        // The AABB is hit if the ray intersects all three axis-aligned intervals.

        const point3&   ray_orig    = r.origin();
        const vec3&     ray_dir     = r.direction();

        for (int axis = 0; axis < 3; ++axis) {
            const interval& ax = axis_interval(axis);
            const double adinv = 1.0 / ray_dir[axis];

            auto t0 = (ax.min - ray_orig[axis]) * adinv;
            auto t1 = (ax.max - ray_orig[axis]) * adinv;

            // Ensure t0 is the intersection with the near plane.
            if (t0 < t1) {
                if (t0 > ray_t.min) ray_t.min = t0;
                if (t1 < ray_t.max) ray_t.max = t1;
            } else {
                if (t1 > ray_t.min) ray_t.min = t1;
                if (t0 < ray_t.max) ray_t.max = t0;
            }

            if (ray_t.max <= ray_t.min)
                return false;
        }
        return true;
    }

    int longest_axis() const {
        // Return the index of the longest axis of the bounding box.
        if (x.size() > y.size())
            return x.size() > z.size() ? 0 : 2;
        else
            return y.size() > z.size() ? 1 : 2;
    }

    static const AABB empty, universe;

 private:

  void pad_to_minimums() {
    // Adjust the AABB so that no side is narrower than some delta, padding if necessary.

    const double delta = 0.0001;
    if (x.size() < delta) x = x.expand(delta);
    if (y.size() < delta) y = y.expand(delta);
    if (z.size() < delta) z = z.expand(delta);
  }
};

const AABB AABB::empty = AABB(interval::empty, interval::empty, interval::empty);
const AABB AABB::universe = AABB(interval::universe, interval::universe, interval::universe);


#endif //RAYTRACINGINONEWEEKEND_AABB_H
