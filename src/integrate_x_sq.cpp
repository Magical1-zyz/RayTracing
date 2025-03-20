//
// Created by ASUS on 2025/3/5.
//
/************************
 * @Author: Magical1
 * @Time: 2025/3/5 20:00
 * @File: integrate_x_sq.cpp
 * @Software: CLion
 * @Project: RayTracingInOneWeekend
 * @Description: Integrate x^2 using Monte Carlo method.
 */


#include "rtweekend.h"

#include <iomanip>

double icd(double d) {
  return 8.0 * std::pow(d, 1.0/3.0);
}

double pdf(double x) {
  return (3.0/8.0) * x * x;
}

int main() {
  int N = 1;
  auto sum = 0.0;

  for (int i = 0; i < N; ++i) {
    auto z = random_double();
    if (z == 0) // Ignore zero to avoid NaNs
      continue;

    auto x = icd(z);
    sum += x * x / pdf(x);
  }
  std::cout << std::fixed << std::setprecision(12);
  std::cout << "I = " << sum / N << '\n';
}