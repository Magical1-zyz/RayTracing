//
// Created by ASUS on 2025/2/16.
//

/************************
 * @Author: Magical1
 * @Time: 2025/2/16 20:00
 * @File: camera.h
 * @Software: CLion
 * @Project: RayTracingInOneWeekend
 * @Description:
 */

#ifndef RAYTRACINGINONEWEEKEND_CAMERA_H
#define RAYTRACINGINONEWEEKEND_CAMERA_H

#include "rtweekend.h"

#include "hittable.h"
#include "pdf.h"
#include "material.h"

class camera {
public:
    double  aspect_ratio       = 1.0;   // Ratio of image width over height
    int     image_width        = 100;   // Rendered image width in pixel count
    int     samples_per_pixel  = 10;    // Number of samples per pixel
    int     max_depth          = 50;    // Maximum ray bounce depth
    color   background_color;           // Scene background color

    double  vfov        = 90;                           // Vertical view angle (field of view) in degrees
    point3  lookfrom    = point3(0, 0, 0);  // Camera location
    point3  lookat      = point3(0, 0, -1); // Camera target
    vec3    vup         = vec3(0, 1, 0);    // Camera up vector

    double  defocus_angle   = 0;    // Variation angle of rays through each pixel
    double  focus_dist      = 10;   // Distance from camera lookfrom to plan of perfect focus


    // Render the world
    void render (const hittable& world, const hittable& lights) {
        initialize();

        // Render

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << "\n"  << std::flush;
            for (int i = 0; i < image_width; i++) {
                color pixel_color(0, 0, 0);
              for (int s_j = 0; s_j < sqrt_spp; ++s_j) {
                for (int s_i = 0; s_i < sqrt_spp; ++s_i) {
                  ray r = get_ray(i, j, s_i, s_j);
                  pixel_color += ray_color(r, max_depth, world, lights);
                }
              }
                write_color(std::cout, pixel_samples_scale*pixel_color);
            }
        }

        std::clog << "\rDone.                 \n";
    }

private:
    int     image_height{};           // Rendered image height
    double  pixel_samples_scale{};    // Color scale factor for a sum of pixel samples
    int     sqrt_spp{};               // Square root of samples per pixel
    double  recip_sqrt_spp{};         // Reciprocal of the square root of samples per pixel: 1/sqrt_spp
    point3  center;                 // Camera center
    point3  pixel00_loc;            // Location of the upper left pixel
    vec3    pixel_delta_u;          // Horizontal delta vector from pixel to pixel
    vec3    pixel_delta_v;          // Vertical delta vector from pixel to pixel
    vec3    u, v, w;                // Camera coordinate system
    vec3    defocus_disk_u;         // Defocus disk horizontal vector
    vec3    defocus_disk_v;         // Defocus disk vertical vector



    // Initialize the camera.
    void  initialize() {
        image_height = int (image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        sqrt_spp = int(std::sqrt(samples_per_pixel));
        pixel_samples_scale = 1.0 / (sqrt_spp * sqrt_spp);
        recip_sqrt_spp = 1.0 / sqrt_spp;

        center = lookfrom;

        // Determine viewport dimensions.
//        auto focal_length       = (lookfrom - lookat).length();
        auto theta              = degrees_to_radians(vfov);
        auto h                  = std::tan(theta/2);
        auto viewport_height    = 2.0 * h * focus_dist;
        auto viewport_width     = viewport_height * (double(image_width)/image_height);

        // Calculate the u,v,w unit basis vectors for the camera coordinate system.
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = viewport_width * u;   // Vector across the horizontal viewport edge
        auto viewport_v = viewport_height * -v;  // Vector down the vertical viewport edge

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left =
                center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate the camera defocus disk basis vectors.
        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle) / 2);
        defocus_disk_u = defocus_radius * u;
        defocus_disk_v = defocus_radius * v;
    }

    [[nodiscard]] ray get_ray(int i, int j, int s_i, int s_j) const {
        // Construct a camera ray origination from the defocus disk and directed at
        // a randomly sampled point around the pixel location i, j for stratified
        // sample square s_i, s_j.

        auto offset = sample_square_stratified(s_i, s_j);
        auto pixel_sample = pixel00_loc
                + ((i + offset.x()) * pixel_delta_u)
                + ((j + offset.y()) * pixel_delta_v);

        auto ray_origin     = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction   = unit_vector(pixel_sample - ray_origin);
        auto ray_time       = random_double();

        return {ray_origin, ray_direction, ray_time};
    }

    [[nodiscard]] vec3 sample_square_stratified(int s_i, int s_j) const {
        // Returns the vector to a random point in the square sub-pixel specified by grid
        // indices s_i and s_j, for an idealized unit square pixel [-.5,-.5] - [+.5,+.5].

        auto px = ((s_i + random_double()) * recip_sqrt_spp) - 0.5;
        auto py = ((s_j + random_double()) * recip_sqrt_spp) - 0.5;

        return {px, py, 0};
    }

    [[nodiscard]] vec3 sample_square() const {
        // Returns the vector to a random point in the [-.5, -.5] - [+.5, +.5] unit square.
        return {random_double() - 0.5, random_double() - 0.5, 0};
    }

    [[nodiscard]] point3 defocus_disk_sample() const {
        // Returns a point on the camera defocus disk.
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    [[nodiscard]] color ray_color(
      const ray& r, int depth, const hittable& world, const hittable& lights) const {
      // The ray_color function takes a ray as an argument and returns a color.

      // If we've exceeded the ray bounce limit, no more light is gathered.
      if (depth <= 0)
            return {0, 0, 0};

      hit_record rec;

      // If the ray hits nothing, return the background color.
      if (!world.hit(r, interval(0.001, infinity), rec))
          return background_color;

      // Create a scatter record to store scattering information
      scatter_record srec;

      // Get the emitted color from the material at the hit point
      color color_from_emission = rec.mat->emitted(r, rec, rec.u, rec.v, rec.p);

      // If the material does not scatter the ray, return the emitted color
      if (!rec.mat->scatter(r, rec, srec))
        return color_from_emission;

      // If the PDF should be skipped, recursively get the color from the scattered ray
      // using the precomputed scattered ray in the scatter record.
      if (srec.skip_pdf)
        return srec.attenuation * ray_color(srec.skip_pdf_ray, depth - 1, world, lights);

      // Create a PDF for sampling light sources
      auto light_ptr =
          make_shared<hittable_pdf>(lights, rec.p);

      // Combine the light source PDF with the material's scattering PDF
      mixture_pdf p(light_ptr, srec.pdf_ptr);

      // Generate a scattered ray using the combined PDF
      ray scattered = ray(rec.p, p.generate(), r.time());

      // Calculate the PDF value for the scattered direction
      auto pdf_value = p.value(scattered.direction());

      // Calculate the scattering PDF value for the scattered direction
      double scattering_pdf = rec.mat->scattering_pdf(r, rec, scattered);

      // Recursively get the color from the scattered ray
      color sample_color = ray_color(scattered, depth - 1, world, lights);

      // Calculate the color contribution from scattering
      color color_from_scatter =
          (srec.attenuation * scattering_pdf * sample_color) / pdf_value;

      // If the ray is scattered, return the scattered ray color and the emitted light.
      return color_from_emission + color_from_scatter;
    }
};
#endif //RAYTRACINGINONEWEEKEND_CAMERA_H
