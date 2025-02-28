//
// Created by ASUS on 2025/2/15.
//

/************************
 * @Author: Magical1
 * @Time: 2025/2/15 20:00
 * @File: ray.h
 * @Software: CLion
 * @Project: RayTracingInOneWeekend
 * @Description:
 */

#ifndef RAYTRACINGINONEWEEKEND_RAY_H
#define RAYTRACINGINONEWEEKEND_RAY_H

#include "rtweekend.h"

class ray {
public:
    ray() = default;

    ray(const point3& origin, const vec3& direction, double time)
        : orig(origin), dir(direction), tm(time) {}

    ray(const point3& origin, const vec3& direction)
        : ray(origin, direction, 0) {}

    const point3& origin() const { return orig; }
    const vec3& direction() const { return dir; }

    double time() const { return tm; }

    point3 at(double t) const {
        return orig + t*dir;
    }

private:
    point3  orig;
    vec3    dir;
    double  tm{};
};
#endif //RAYTRACINGINONEWEEKEND_RAY_H
