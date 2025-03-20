//
// Created by ASUS on 2025/2/16.
//

/************************
 * @Author: Magical1
 * @Time: 2025/2/16 20:00
 * @File: hittable_list.h
 * @Software: CLion
 * @Project: RayTracingInOneWeekend
 * @Description:
 */

#ifndef RAYTRACINGINONEWEEKEND_HITTABLE_LIST_H
#define RAYTRACINGINONEWEEKEND_HITTABLE_LIST_H

#include "rtweekend.h"

#include "AABB.h"
#include "hittable.h"



// The hittable_list class stores a list of hittable objects.
class hittable_list : public hittable {
public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() = default;
    explicit hittable_list(const shared_ptr<hittable>& object) { add(object); }

    void clear() { objects.clear(); }

    void add(const shared_ptr<hittable>& object) {
        objects.push_back(object);
        bbox = AABB(bbox, object->bounding_box());
    }

    bool hit (const ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for (const auto& object : objects) {
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }

    [[nodiscard]] AABB bounding_box() const override {
        return bbox;
    }

    // Calculates the probability density function (PDF) value for a ray from origin
    // in given direction by averaging the PDF values of all objects in the list.
    // origin: The starting point of the ray
    // direction: The direction vector of the ray
    // Returns: The weighted average of PDF values from all objects
    [[nodiscard]] double pdf_value(const point3& origin, const vec3& direction) const override {
      auto weight = 1.0 / objects.size();  // Weight for each object's contribution
      auto sum = 0.0;  // Accumulator for weighted PDF values

      for (const auto& object : objects)
          sum += weight * object->pdf_value(origin, direction);

      return sum;
    }

    // Generates a random direction by randomly selecting an object and using its random direction
    // origin: The point from which to generate the random direction
    // Returns: A random direction vector from the selected object
    [[nodiscard]] vec3 random(const point3& origin) const override {
      auto int_size = static_cast<int>(objects.size());  // Convert size to int for random selection
      return objects[random_int(0, int_size - 1)]->random(origin);  // Get random direction from randomly selected object
    }

private:
    AABB bbox;
};

#endif //RAYTRACINGINONEWEEKEND_HITTABLE_LIST_H
