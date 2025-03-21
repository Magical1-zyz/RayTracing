//
// Created by ASUS on 2025/2/15.
//

/************************
 * @Author: Magical1
 * @Time: 2025/2/15 20:00
 * @File: vec3.h
 * @Software: CLion
 * @Project: RayTracingInOneWeekend
 * @Description:
 */

#ifndef RAYTRACINGINONEWEEKEND_VEC3_H
#define RAYTRACINGINONEWEEKEND_VEC3_H

#include "rtweekend.h"

class vec3 {
public :
    double  e[3];

    vec3() : e{0, 0, 0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return {-e[0], -e[1], -e[2]}; }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(const vec3 &v) {
        e[0] *= v.e[0];
        e[1] *= v.e[1];
        e[2] *= v.e[2];
        return *this;
    }

    vec3& operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(const double t) {
        return *this *= 1/t;
    }

    double length() const {
        return std::sqrt(length_squared());
    }

    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        auto s = 1e-8;
        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    }

    static vec3 random() {
        // Generate arbitrary random vectors.
        return {random_double(), random_double(), random_double()};
    }

    static vec3 random(double min, double max) {
        // Generate a random vector with each component in the range [min, max).
        return {random_double(min, max),
                    random_double(min, max), random_double(min, max)};
    }
};

using point3 = vec3;


// vec3 Utility Functions

inline std::ostream & operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return {u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]};
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return {u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]};
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return {u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]};
}

inline vec3 operator*(double t, const vec3 &v) {
    return {t*v.e[0], t*v.e[1], t*v.e[2]};
}

inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return {u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]};
}

inline vec3 unit_vector(vec3 v) {
    // Returns the unit vector of v.
    return v / v.length();
}

inline vec3 random_in_unit_disk() {
    // Returns a random vector in the unit disk.
    while (true) {
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() < 1)
            return p;
    }
}

inline vec3 random_unit_vector() {
    // Returns a random unit vector.
    while (true) {
       auto p = vec3::random(-1, 1);
       auto lensq = p.length_squared();
       // Rejection Sampling:Ensure that the subsequent
       // normalised vectors are uniformly distributed on the sphere.
       // 1e-160:to avoid division by zero.
        if (1e-160 < lensq && lensq <= 1)
            return p / std::sqrt(lensq);
    }
}

inline vec3 random_on_hemisphere(const vec3& normal) {
    // Determine a random point on the correct hemisphere.
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

inline vec3 random_cosine_direction() {
    // Generate a random cosine-weighted direction.
    auto r1 = random_double();
    auto r2 = random_double();

    auto phi = 2 * pi * r1;
    auto x = std::cos(phi) * std::sqrt(r2);
    auto y = std::sin(phi) * std::sqrt(r2);
    auto z = std::sqrt(1 - r2);

    return {x, y, z};
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    // Reflects a vector v around a normal n.
    return v - 2*dot(v, n)*n;
}

inline vec3 refract(const vec3& uv, const vec3& n,double etai_over_etat) {
    // Refracts a vector uv around a normal n.
    auto cos_theta = std::fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

#endif //RAYTRACINGINONEWEEKEND_VEC3_H
