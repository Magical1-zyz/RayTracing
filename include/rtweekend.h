//
// Created by ASUS on 2025/2/16.
//

/************************
 * @Author: Magical1
 * @Time: 2025/2/16 20:00
 * @File: rtweekend.h
 * @Software: CLion
 * @Project: RayTracingInOneWeekend
 * @Description:
 */

#ifndef RAYTRACINGINONEWEEKEND_RTWEEKEND_H
#define RAYTRACINGINONEWEEKEND_RTWEEKEND_H

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <utility>

// C++ Std Usings

using std::make_shared;
using std::shared_ptr;

// Constants

const double infinity   = std::numeric_limits<double>::infinity();
const double pi         = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(double degrees) {
    // Converts degrees to radians.
    return degrees * pi  / 180.0;
}

inline double random_double() {
    // Returns a random real in [0,1).
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}

inline int random_int(int min, int max) {
    // Returns a random integer in [min,max].
    return int(random_double(min,max+1));
}

// Common Headers
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif //RAYTRACINGINONEWEEKEND_RTWEEKEND_H
