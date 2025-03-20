//
// Created by ASUS on 2025/3/6.
//
/************************
 * @Author: Magical1
 * @Time: 2025/3/6 20:00
 * @File: sphere_importance.cpp
 * @Software: CLion
 * @Project: RayTracingInOneWeekend
 * @Description: Importance sampling for a sphere.
 */

#include "rtweekend.h"

#include <iomanip>

double  f(const vec3& d) {
  auto cosine_squared = d.z() * d.z();
  return cosine_squared;
}

double pdf(const vec3& d) {
  return 1 / (4*pi);
}

int main(){
  int N = 1000000;
  auto sum = 0.0;
  for (int i = 0; i < N; ++i) {
    vec3 d = random_unit_vector();
    auto f_d = f(d);
    sum += f_d / pdf(d);
  }
  std::cout << std::fixed << std::setprecision(12);
  std::cout << "I = " << sum / N << '\n';
}