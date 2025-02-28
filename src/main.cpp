//
// Created by ASUS on 2025/2/16.
//

/************************
 * @Author: Magical1
 * @Time: 2025/2/16 20:00
 * @File: main.cpp
 * @Software: CLion
 * @Project: RayTracingInOneWeekend
 * @Description:Main file for the Ray Tracing Project.
 */

#include "rtweekend.h"

#include "bvh.h"
#include "camera.h"
#include "constant_medium.h"
#include "hittable_list.h"
#include "material.h"
#include "quad.h"
#include "sphere.h"
#include "texture.h"

#include <chrono>

void bouncing_spheres() {
  // World
  hittable_list world;

  // Ground Sphere
  auto ground_material = make_shared<lambertian>(
      color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(
      point3(0, -1000, 0), 1000, ground_material));

  // Random Spheres
  for (int a = -11; a < 11; ++a) {
    for (int b = -11; b < 11; ++b) {
      auto choose_mat = random_double();
      point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<material> sphere_material;

        if (choose_mat < 0.8) {
          // Diffuse
          auto albedo = color::random() * color::random();
          sphere_material = make_shared<lambertian>(albedo);
          // Sphere movement
          auto center2 = center + vec3(0, random_double(0, 0.5), 0);
          world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // Metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = make_shared<metal>(albedo, fuzz);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else {
          // Glass
          sphere_material = make_shared<dielectric>(1.5);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  // Three Large Spheres
  auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

  // BVH Acceleration
  world = hittable_list(make_shared<bvh_node>(world));

  // Camera
  camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 1200;
  cam.samples_per_pixel = 100;
  cam.max_depth = 50;
  cam.background_color  = color(0.70, 0.80, 1.00);

  cam.vfov = 20;
  cam.lookfrom = point3(13, 2, 3);
  cam.lookat = point3(0, 0, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0.6;
  cam.focus_dist = 10.0;

  // Render
  cam.render(world);
}

void checkered_spheres() {
  // World
  hittable_list world;

  // Checkerboard Ground
  auto checker = make_shared<checker_texture>(0.32,
                                              color(0.2, 0.3, 0.1),
                                              color(0.9, 0.9, 0.9));
  world.add(make_shared<sphere>(point3(0,-10,0),
                                10, make_shared<lambertian>(checker)));
  world.add(make_shared<sphere>(point3(0,  10, 0),
                                10, make_shared<lambertian>(checker)));



  // BVH Acceleration
  world = hittable_list(make_shared<bvh_node>(world));

  // Camera
  camera cam;

  cam.aspect_ratio        = 16.0 / 9.0;
  cam.image_width         = 1200;
  cam.samples_per_pixel   = 100;
  cam.max_depth           = 50;
  cam.background_color  = color(0.70, 0.80, 1.00);

  cam.vfov                = 20;
  cam.lookfrom            = point3(13, 2, 3);
  cam.lookat              = point3(0, 0, 0);
  cam.vup                 = vec3(0, 1, 0);

  cam.defocus_angle       = 0;


  // Render
  cam.render(world);
}

void earth() {
  // Globe
  auto earth_texture = make_shared<image_texture>(
      "../assets/textures/earthmap.jpg");
  auto earth_surface = make_shared<lambertian>(earth_texture);
  auto globe = make_shared<sphere>(
      point3(0, 0, 0), 2, earth_surface);

  // Camera
  camera cam;

  cam.aspect_ratio      = 16.0 / 9.0;
  cam.image_width       = 1200;
  cam.samples_per_pixel = 100;
  cam.max_depth         = 50;
  cam.background_color  = color(0.70, 0.80, 1.00);

  cam.vfov              = 20;
  cam.lookfrom          = point3(0, 0, 12);
  cam.lookat            = point3 (0, 0, 0);
  cam.vup               = vec3(0, 1, 0);

  cam.defocus_angle     = 0;

  cam.render(hittable_list(globe));
}

void perlin_spheres() {
  // World
  hittable_list world;

  auto pertext = make_shared<noise_texture>(4);
  world.add(make_shared<sphere>(
      point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
  world.add(make_shared<sphere>(
      point3(0,2,0), 2, make_shared<lambertian>(pertext)));

  // BVH Acceleration
  world = hittable_list(make_shared<bvh_node>(world));

  // Camera
  camera cam;
  cam.aspect_ratio      = 16.0 / 9.0;
  cam.image_width       = 1200;
  cam.samples_per_pixel = 100;
  cam.max_depth         = 50;
  cam.background_color  = color(0.70, 0.80, 1.00);

  cam.vfov              = 20;
  cam.lookfrom          = point3(13, 2, 3);
  cam.lookat            = point3 (0, 0, 0);
  cam.vup               = vec3(0, 1, 0);

  cam.defocus_angle     = 0;

  // Render
  cam.render(world);
}

void quads() {
  // World
  hittable_list world;

  // Materials
  auto left_red     = make_shared<lambertian>(color(1.0, 0.2, 0.2));
  auto back_green   = make_shared<lambertian>(color(0.2, 1.0, 0.2));
  auto right_blue   = make_shared<lambertian>(color(0.2, 0.2, 1.0));
  auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
  auto lower_teal   = make_shared<lambertian>(color(0.2, 0.8, 0.8));

  // Quads
  world.add(make_shared<quad>(point3(-3, -2, 5), vec3(0, 0, -4),
                              vec3(0, 4, 0), left_red));
  world.add(make_shared<quad>(point3(-2, -2, 0), vec3(4, 0, 0),
                              vec3(0, 4, 0), back_green));
  world.add(make_shared<quad>(point3(3, -2, 1), vec3(0, 0, 4),
                              vec3(0, 4, 0), right_blue));
  world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0),
                              vec3(0, 0, 4), upper_orange));
  world.add(make_shared<quad>(point3(-2, -3, 5), vec3(4, 0, 0),
                              vec3(0, 0, -4), lower_teal));

  // BVH Acceleration
  world = hittable_list(make_shared<bvh_node>(world));

  // Camera
  camera cam;
  cam.aspect_ratio      = 1.0;
  cam.image_width       = 1200;
  cam.samples_per_pixel = 100;
  cam.max_depth         = 50;
  cam.background_color  = color(0.70, 0.80, 1.00);

  cam.vfov              = 80;
  cam.lookfrom          = point3(0, 0, 9);
  cam.lookat            = point3 (0, 0, 0);
  cam.vup               = vec3(0, 1, 0);

  cam.defocus_angle     = 0;

  // Render
  cam.render(world);
}

void simple_light() {
  // World
  hittable_list world;

  // Materials
  auto pertext = make_shared<noise_texture>(4);
  world.add(make_shared<sphere>(
      point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
  world.add(make_shared<sphere>(
      point3(0,2,0), 2, make_shared<lambertian>(pertext)));

  auto difflight = make_shared<diffuse_light>(color(4,4,4));
  world.add(make_shared<quad>(point3(3,1,-2), vec3(2, 0, 0),
                              vec3(0, 2, 0), difflight));

  // BVH Acceleration
  world = hittable_list(make_shared<bvh_node>(world));

  // Camera
  camera cam;
  cam.aspect_ratio      = 16.0 / 9.0;
  cam.image_width       = 1200;
  cam.samples_per_pixel = 100;
  cam.max_depth         = 50;
  cam.background_color  = color(0, 0, 0);

  cam.vfov              = 20;
  cam.lookfrom          = point3(26, 3, 6);
  cam.lookat            = point3 (0, 2, 0);
  cam.vup               = vec3(0, 1, 0);

  cam.defocus_angle     = 0;

  // Render
  cam.render(world);
}

void cornell_box() {
  // World
  hittable_list world;

  // Materials
  auto red   = make_shared<lambertian>(color(.65, .05, .05));
  auto white = make_shared<lambertian>(color(.73, .73, .73));
  auto green = make_shared<lambertian>(color(.12, .45, .15));
  auto light = make_shared<diffuse_light>(color(15, 15, 15));

  // Cornell Box
  world.add(make_shared<quad>(point3(555, 0, 0), vec3(0, 0, 555),
                              vec3(0, 555, 0), green));
  world.add(make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0),
                              vec3(0, 0, 555), red));
  world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130, 0, 0),
                              vec3(0, 0, -105), light));
  world.add(make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0),
                              vec3(0, 0, 555), white)); // bottom
  world.add(make_shared<quad>(point3(555, 555, 555), vec3(-555, 0, 0),
                              vec3(0, 0, -555), white)); // top
  world.add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0),
                              vec3(0, 555, 0), white)); // back

  // boxes
  shared_ptr<hittable> box1 = box(point3(0, 0, 0),
                                  point3(165, 330, 165), white);
  box1 = make_shared<rotate_y>(box1, 15);
  box1 = make_shared<translate>(box1, vec3(265, 0, 295));
  world.add(box1);

  shared_ptr<hittable> box2 = box(point3(0, 0, 0),
                                  point3(165, 165, 165), white);
  box2 = make_shared<rotate_y>(box2, -18);
  box2 = make_shared<translate>(box2, vec3(130, 0, 65));
  world.add(box2);

  // BVH Acceleration
  world = hittable_list(make_shared<bvh_node>(world));

  // Camera
  camera cam;

  cam.aspect_ratio      = 1.0;
  cam.image_width       = 600;
  cam.samples_per_pixel = 200;
  cam.max_depth         = 50;
  cam.background_color  = color(0, 0, 0);

  cam.vfov              = 40;
  cam.lookfrom          = point3(278, 278, -800);
  cam.lookat            = point3 (278, 278, 0);
  cam.vup               = vec3(0, 1, 0);

  cam.defocus_angle     = 0;

  // Render
  cam.render(world);
}

void cornell_smoke() {
  // World
  hittable_list world;

  // Materials
  auto red    = make_shared<lambertian>(color (.65, .05, .05));
  auto white  = make_shared<lambertian>(color (.73, .73, .73));
  auto green  = make_shared<lambertian>(color (.12, .45, .15));
  auto light = make_shared<diffuse_light>(color(7, 7, 7));

  // Cornell Box
  world.add(make_shared<quad>(point3(555, 0, 0), vec3(0, 0, 555),
                              vec3(0, 555, 0), green));
  world.add(make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0),
                              vec3(0, 0, 555), red));
  world.add(make_shared<quad>(point3(113, 554, 127), vec3(330, 0, 0),
                              vec3(0, 0, 305), light));
  world.add(make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0),
                              vec3(0, 0, 555), white)); // bottom
  world.add(make_shared<quad>(point3(555, 555, 555), vec3(-555, 0, 0),
                              vec3(0, 0, -555), white)); // top
  world.add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0),
                              vec3(0, 555, 0), white)); // back

  // boxes
  shared_ptr<hittable> box1 = box(point3(0, 0, 0),
                                  point3(165, 330, 165), white);
  box1 = make_shared<rotate_y>(box1, 15);
  box1 = make_shared<translate>(box1, vec3(265, 0, 295));

  shared_ptr<hittable> box2 = box(point3(0, 0, 0),
                                  point3(165, 165, 165), white);
  box2 = make_shared<rotate_y>(box2, -18);
  box2 = make_shared<translate>(box2, vec3(130, 0, 65));

  world.add(make_shared<constant_medium>(box1, 0.01,
                                         color(0, 0, 0)));
  world.add(make_shared<constant_medium>(box2, 0.01,
                                         color(1, 1, 1)));

  // BVH Acceleration
  world = hittable_list(make_shared<bvh_node>(world));

  // Camera
  camera cam;

  cam.aspect_ratio      = 1.0;
  cam.image_width       = 600;
  cam.samples_per_pixel = 200;
  cam.max_depth         = 50;
  cam.background_color  = color(0, 0, 0);

  cam.vfov              = 40;
  cam.lookfrom          = point3(278, 278, -800);
  cam.lookat            = point3 (278, 278, 0);
  cam.vup               = vec3(0, 1, 0);

  cam.defocus_angle     = 0;

  // Render
  cam.render(world);
}

int main() {
    // time
    auto start_time = std::chrono::high_resolution_clock::now();

  switch (8) {
    // Choose the scene to render.
    case 1: // Bouncing Spheres
      bouncing_spheres();
      break;
    case 2: // Checkered Spheres
      checkered_spheres();
      break;
    case 3: // Earth
      earth();
      break;
    case 4: // Perlin Spheres
        perlin_spheres();
        break;
    case 5: // Quads
        quads();
        break;
    case 6: // A simple rectangle light
        simple_light();
        break;
    case 7: // Cornell Box
        cornell_box();
        break;
    case 8: // Cornell Box with Smoke
        cornell_smoke();
        break;
  }

    // end time
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end_time - start_time;
    std::clog << "Elapsed time: " << elapsed_time.count() << "s\n";

}
