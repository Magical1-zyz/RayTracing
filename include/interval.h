//
// Created by ASUS on 2025/2/16.
//

/************************
 * @Author: Magical1
 * @Time: 2025/2/16 20:00
 * @File: interval.h
 * @Software: CLion
 * @Project: RayTracingInOneWeekend
 * @Description:
 */

#ifndef RAYTRACINGINONEWEEKEND_INTERVAL_H
#define RAYTRACINGINONEWEEKEND_INTERVAL_H

class interval {
public:
    double min, max;

    interval() : min(+infinity), max(-infinity) {} // Default interval  is empty.

    interval(double min, double max) : min(min), max(max) {}

    interval(const interval&a, const interval&b) {
        // Create the interval tightly enclosing the two input intervals.
        min = a.min <= b.min ? a.min : b.min;
        max = a.max >= b.max ? a.max : b.max;
    }

    double size() const {
        return max - min;
    }

    bool contains(double x) const {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const {
        return min < x && x < max;
    }

    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    interval expand(double delta) const {
        // Expand the interval by a given delta to both sides.
        auto half_delta = 0.5 * delta;
        return interval(min - half_delta, max + half_delta);
    }

    static const interval empty, universe;
};

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

#endif //RAYTRACINGINONEWEEKEND_INTERVAL_H
