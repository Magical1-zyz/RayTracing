//
// Created by ASUS on 2025/3/12.
//
/************************
 * @Author: Magical1
 * @Time: 2025/3/12 20:00
 * @File: cos_cubed.cpp
 * @Software: CLion
 * @Project: RayTracingInOneWeekend
 * @Description: Random points on the unit sphere
 */

#include "rtweekend.h"

#include <cmath>

int main() {
  for (int i = 0; i < 200; ++i) {
    auto  r1 = random_double();
    auto  r2 = random_double();
    auto x = 2 * std::cos(2*pi*r1) * std::sqrt(r2*(1-r2));
    auto y = 2 * std::sin(2*pi*r1) * std::sqrt(r2*(1-r2));
    auto z = 1 - 2*r2;
    std::cout << x << ' ' << y << ' ' << z << '\n';
  }
}

