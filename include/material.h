//
// Created by ASUS on 2025/2/18.
//

/************************
 * @Author: Magical1
 * @Time: 2025/2/18 12:00
 * @File: material.h
 * @Software: CLion
 * @Project: RayTracingInOneWeekend
 * @Description:
 */

#ifndef RAYTRACINGINONEWEEKEND_MATERIAL_H
#define RAYTRACINGINONEWEEKEND_MATERIAL_H

#include "hittable.h"

class material {
    // The material class is an abstract base class for materials.
    // It provides a scatter function that returns a scattered ray
    // or indicates that the ray was absorbed.
public:
    virtual ~material() = default;

    virtual bool scatter (
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
            ) const {
        return false;
    }
};

class lambertian : public material {
    // The lambertian class represents a diffuse, or matte, material.
public:
    lambertian(const color& albedo) : albedo(albedo) {}

    bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
            ) const override {
        // Scatter a ray in a random direction.
        auto scatter_direction = rec.normal + random_unit_vector();

        // Catch degenerate scatter direction.
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;

        return true;
    }
private:
    color albedo; // The albedo of the material.
};

class metal : public material {
    // The metal class represents a reflective material.
public:
    metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
            ) const override {
        // Reflect the ray around the normal.
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        reflected = unit_vector(reflected) + fuzz * random_unit_vector();
        scattered = ray(rec.p, reflected);
        attenuation = albedo;

        return (dot(scattered.direction(), rec.normal) > 0);
    }

private:
    color   albedo; // The albedo of the material.
    double  fuzz;   // The fuzziness of the reflection.
};

#endif //RAYTRACINGINONEWEEKEND_MATERIAL_H
