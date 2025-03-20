//
// Created by ASUS on 2025/3/5.
//
/************************
 * @Author: Magical1
 * @Time: 2025/3/5 20:00
 * @File: estimate_halfway.cpp
 * @Software: CLion
 * @Project: RayTracingInOneWeekend
 * @Description: Estimate the halfway point of an interval.
 */
#include "rtweekend.h"

#include <algorithm>
#include <vector>
#include <iomanip>

struct sample {
    double x;
    double p_x;
};

bool compare_by_x(const sample& a, const sample& b) {
    return a.x < b.x;
}

int main() {
  const unsigned int N = 10000;
  sample samples[N];
  double sum = 0.0;

  // Iterate through all of our samples

  for (auto & i : samples) {
    // Get the area under the curve.
    auto x = random_double(0, 2*pi);
    auto sin_x = std::sin(x);
    auto p_x = exp(-x / (2*pi)) * sin_x * sin_x;
    sum += p_x;

    // Store the sample
    sample this_sample = {x, p_x};
    i = this_sample;
  }

  // Sort the samples by x.
  std::sort(std::begin(samples), std::end(samples), compare_by_x);

  // Find out the sample at which we have half of our area.
  double half_area = sum / 2;
  double halfway_point = 0.0;
  double accum = 0.0;
    for (auto & sample : samples) {
        accum += sample.p_x;
        if (accum > half_area) {
          halfway_point = sample.x;
          break;
        }
    }

  std::cout << std::fixed << std::setprecision(12);
  std::cout << "Average: " << sum / N << std::endl;
  std::cout << "Area under the curve:" << 2 * pi * sum / N << std::endl;
  std::cout << "Halfway point: " << halfway_point << std::endl;
}