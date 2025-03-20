//
// Created by ASUS on 2025/2/15.
//

/************************
 * @Author: Magical1
 * @Time: 2025/2/15 20:00
 * @File: color.h
 * @Software: CLion
 * @Project: RayTracingInOneWeekend
 * @Description:
 */

#ifndef RAYTRACINGINONEWEEKEND_COLOR_H
#define RAYTRACINGINONEWEEKEND_COLOR_H

#include "rtweekend.h"

#include "interval.h"
#include "vec3.h"

using color = vec3;

// Converts linear RGB color component to gamma-corrected component using gamma 2.0
inline double linear_to_gamma(double linear_component) {
    if (linear_component > 0)
        return std::sqrt(linear_component);  // Apply gamma correction using sqrt (gamma 2.0)
    return 0;  // Handle non-positive values
}

// Writes a color to the output stream in PPM format
// Handles color clamping, gamma correction, and NaN values
void write_color(std::ostream &out, const color& pixel_color) {
    // Extract RGB components from the color vector
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Replace NaN components with zero
    // NaN comparison with itself returns false, so (x != x) identifies NaN values
    if (r != r) r = 0.0;
    if (g != g) g = 0.0;
    if (b != b) b = 0.0;

    // Apply gamma correction (gamma 2.0) to convert from linear to display color space
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Clamp color values to [0,0.999] and convert to 8-bit integers [0,255]
    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    // Write color components as space-separated integers with newline
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif //RAYTRACINGINONEWEEKEND_COLOR_H
