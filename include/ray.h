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
    ray() {}

    ray(const point3& origin, const vec3& direction)
        : orig(origin), dir(direction)
    {}

    const point3& origin() const { return orig; }
    const vec3& direction() const { return dir; }

    point3 at(double t) const {
        return orig + t*dir;
    }

private:
    point3  orig;
    vec3    dir;
};
#endif //RAYTRACINGINONEWEEKEND_RAY_H
