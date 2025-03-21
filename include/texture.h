//
// Created by ASUS on 2025/2/23.
//
/************************
 * @Author: Magical1
 * @Time: 2025/2/23 20:00
 * @File: texture.h
 * @Software: CLion
 * @Project: RayTracingInOneWeekend
 * @Description:
 */

#ifndef RAYTRACINGINONEWEEKEND_TEXTURE_H
#define RAYTRACINGINONEWEEKEND_TEXTURE_H

#include "rtweekend.h"

#include "perlin.h"
#include "rtw_stb_image.h"


class texture {
    // The texture class is an abstract base class for textures.
public:
    virtual  ~texture() = default;

    // Return the color of the texture at the given UV coordinates.
    virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {
    // The solid_color class represents a solid color texture.
public:
        explicit solid_color(const color& albedo) : albedo(albedo) {}

        solid_color(double red, double green, double blue)
            : solid_color(color(red, green, blue)) {}

        color value(double u, double v, const point3& p) const override {
            return albedo;
        }

private:
    color albedo; // The albedo of the texture.
};

class checker_texture : public texture {
    // The checker_texture class represents a checkerboard texture.
public:
    checker_texture(double scale, shared_ptr<texture> even, shared_ptr<texture> odd)
      : inv_scale(1.0/scale), even(std::move(even)), odd(std::move(odd)) {}

    checker_texture(double scale, color c1, color c2)
      : checker_texture(scale, make_shared<solid_color>(c1),
          make_shared<solid_color>(c2)) {}

    color value(double u, double v, const point3& p) const override {
        auto xInteger = int(std::floor(inv_scale * p.x()));
        auto yInteger = int(std::floor(inv_scale * p.y()));
        auto zInteger = int(std::floor(inv_scale * p.z()));

        bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
      }

private:
    double              inv_scale;  // To control the size of the checkerboard.
    shared_ptr<texture> even;       // The even texture.
    shared_ptr<texture> odd;        // The odd texture.

};

class image_texture : public texture {
  // The image_texture class represents an image texture.
public:
  explicit image_texture(const char* filename) : image(filename) {}

  color value(double u, double v, const point3& p) const override {
    // If we have no texture data, then return solid cyan as a debugging aid.
    if (image.height() <= 0) return {0, 1, 1};

    // Clamp input texture coordinates to [0,1] x [1,0]
    u = interval(0, 1).clamp(u);
    v = 1.0 - interval(0, 1).clamp(v); // Flip V to image coordinates

    auto i = int(u * image.width());
    auto j = int(v * image.height());
    auto pixel = image.pixel_data(i, j);

    auto color_scale = 1.0 / 255.0;
    return {color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]};
  }

private:
    rtw_image image;
};

class noise_texture : public texture {
  // The noise_texture class represents a noise texture.
public:
  explicit noise_texture(double scale) : scale(scale) {}

  color value(double u, double v, const point3& p) const override {
//    // 0.5 * (1.0 + noise.noise(scale * p)): map the [-1,+1] noise value to [0,1].
//    return color(1, 1, 1) * 0.5 * (1.0 + noise.noise(scale * p));
      return color(.5, .5, .5) * (1.0 + std::sin(scale * p.z() + 10 * noise.turb(p, 7)));
  }

private:
  perlin noise; // The noise object.
  double scale; // The scale of the noise.
};
#endif //RAYTRACINGINONEWEEKEND_TEXTURE_H
