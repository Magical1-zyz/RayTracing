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
#include "onb.h"
#include "pdf.h"
#include "texture.h"


class scatter_record {
    // The scatter_record class stores the scattered ray, the attenuation,
    // and the probability density function value.
 public:
  color           attenuation;  // The attenuation of the scattered ray.
  shared_ptr<pdf> pdf_ptr;      // The probability density function.
  bool            skip_pdf;     // Skip the probability density function.
  ray             skip_pdf_ray; // The ray to skip the probability density function.
};

class material {
    // The material class is an abstract base class for materials.
    // It provides a scatter function that returns a scattered ray
    // or indicates that the ray was absorbed.
public:
    virtual ~material() = default;

    [[nodiscard]] virtual color emitted(
        const ray& r_in, const hit_record& rec, double u, double v, const point3& p) const {
        return {0, 0, 0};
    }

    virtual bool scatter (
            const ray& r_in, const hit_record& rec, scatter_record& srec
            ) const {
        return false;
    }

    // The scattering_pdf function returns the probability density function
    [[nodiscard]] virtual double scattering_pdf(
            const ray& r_in, const hit_record& rec, const ray& scattered
            ) const {
        return 0;
    }
};

class lambertian : public material {
    // The lambertian class represents a diffuse, or matte, material.
public:
    explicit lambertian(const color& albedo) : tex(make_shared<solid_color>(albedo)) {}
    explicit lambertian(shared_ptr<texture> a) : tex(std::move(a)) {}

    bool scatter(
            const ray& r_in, const hit_record& rec, scatter_record& srec
            ) const override {
      // Set the attenuation color for the scattered ray
      srec.attenuation = tex->value(rec.u, rec.v, rec.p);

      // Set the probability density function for the scattered ray
      srec.pdf_ptr = make_shared<cosine_pdf>(rec.normal);

      // Indicate that the PDF should not be skipped
      srec.skip_pdf = false;

      // Indicate that the scattering was successful
      return true;
    }

    // The scattering_pdf function returns the probability density function
    [[nodiscard]] double scattering_pdf(
            const ray& r_in, const hit_record& rec, const ray& scattered
            ) const override {
        // Calculate the cosine of the angle between the surface normal
        // and the scattered ray direction
        auto cos_theta = dot(rec.normal, unit_vector(scattered.direction()));

        // Return the PDF value for the scattered direction
        // If the cosine is negative, return 0 (indicating no scattering in that direction)
        // Otherwise, return the cosine divided by pi (for cosine-weighted hemisphere sampling)
        return cos_theta < 0 ? 0 : cos_theta / pi;
    }

private:
//    color albedo; // The albedo of the material.
    shared_ptr<texture> tex;
};

class metal : public material {
    // The metal class represents a reflective material.
public:
    metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(
            const ray& r_in, const hit_record& rec, scatter_record& srec
            ) const override {
        // Reflect the ray around the normal.
        vec3 reflected = reflect(r_in.direction(), rec.normal);

        // Add fuzziness to the reflection by adding a random unit vector scaled by the fuzz factor.
        reflected = unit_vector(reflected) + fuzz * random_unit_vector();

        // Set the attenuation color for the scattered ray.
        srec.attenuation = albedo;

        // No probability density function is used for this material.
        srec.pdf_ptr = nullptr;

        // Indicate that the PDF should be skipped.
        srec.skip_pdf = true;

        // Set the scattered ray to the reflected ray.
        srec.skip_pdf_ray = ray(rec.p, reflected, r_in.time());

        // Indicate that the scattering was successful.
        return true;
    }

private:
    color   albedo; // The albedo of the material.
    double  fuzz;   // The fuzziness of the reflection.
};

class  dielectric : public material {
    // The dielectric class represents a transparent material.
public:
    explicit dielectric(double refraction_index) : refraction_index(refraction_index) {}

    bool scatter(
            const ray& r_in, const hit_record& rec, scatter_record& srec
            ) const override {
        srec.attenuation  = color(1.0, 1.0, 1.0);
        srec.pdf_ptr      = nullptr;
        srec.skip_pdf     = true;
        // To judge the ray is inside or outside the object.
        double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

        vec3    unit_direction = unit_vector(r_in.direction());
        double  cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
        double  sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

        bool    cannot_refract = ri * sin_theta > 1.0;
        vec3    direction;

        // Determining if the ray can refract.
        if (cannot_refract || reflectance(cos_theta, ri) > random_double())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, ri);

        srec.skip_pdf_ray = ray(rec.p, direction,r_in.time());
        return true;
    }

private:
    // Refractive index in vacuum or air, or the ratio of the material's refractive index over
    // the refractive index of the enclosing media
    double refraction_index; // The refractive index of the material.

    static double reflectance(double cosine, double refraction_index) {
        // Schlick's approximation for reflectance.
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};

class diffuse_light : public material {
    // The diffuse_light class represents a light-emitting material.
public:
  explicit diffuse_light(shared_ptr<texture> tex) : tex(std::move(tex)) {}
  explicit diffuse_light(const color& emit) : tex(make_shared<solid_color>(emit)) {}

  [[nodiscard]] color emitted(
      const ray& r_in, const hit_record& rec, double u, double v, const point3& p) const override{

    // If the ray is hitting the back face of the surface, return black color (no emission).
    if (!rec.front_face)
      return {0, 0, 0};
    return tex->value(u, v, p);
  }

private:
    shared_ptr<texture> tex;
};

class isotropic : public material {
  // The isotropic class represents a material that scatters light in all directions.
 public:
  explicit isotropic(const color& albedo) : tex(make_shared<solid_color>(albedo)) {}
  explicit isotropic(shared_ptr<texture> tex) : tex(std::move(tex)) {}

  bool scatter(
      const ray& r_in, const hit_record& rec, scatter_record& srec
      ) const override {
    // Set the attenuation color for the scattered ray
    srec.attenuation = tex->value(rec.u, rec.v, rec.p);

    // Set the probability density function for the scattered ray
    srec.pdf_ptr = make_shared<sphere_pdf>();

    // Indicate that the PDF should not be skipped
    srec.skip_pdf = false;

    // Indicate that the scattering was successful
    return true;
  }

  [[nodiscard]] double scattering_pdf(
      const ray& r_in, const hit_record& rec, const ray& scattered
      ) const override {
    return 1 / (4 * pi);
  }

 private:
  shared_ptr<texture> tex;
};
#endif //RAYTRACINGINONEWEEKEND_MATERIAL_H
