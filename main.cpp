#include <iostream>

#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "utils.h"
#include "vec3.h"

#define ASPECT_RATIO (16.0f / 9.0f)

color ray_color(const ray &r, const hittable &world) {
  hit_record rec;
  if (world.hit(r, 0, infinity, rec)) {
    return 0.5 * (rec.normal + color(1, 1, 1));
  }

  vec3 unit_direction = unit_vec(r.direction());
  double a = 0.5 * (unit_direction.y() + 1.0);

  return (1.0 - a) * color(1, 1, 1) + a * color(0.5, 0.7, 1.0);
}

int main() {
  int image_width = 400;
  int image_height = static_cast<int>(image_width / ASPECT_RATIO);
  image_height = (image_height < 1) ? 1 : image_height;

  hittable_list world;
  world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));
  world.add(make_shared<sphere>(point3(0,0,-1), 0.5));

  double focal_length = 1.0f;
  double viewport_height = 2.0f;
  double viewport_width =
      viewport_height * (static_cast<double>(image_width) / image_height);
  point3 camera_center = point3(0, 0, 0);

  vec3 viewport_u = vec3(viewport_width, 0, 0);
  vec3 viewport_v = vec3(0, -viewport_height, 0);

  vec3 pixel_delta_u = viewport_u / image_width;
  vec3 pixel_delta_v = viewport_v / image_height;

  point3 viewport_upper_left = camera_center - vec3(0, 0, focal_length) -
                               viewport_u / 2 - viewport_v / 2;

  point3 pixel00_location =
      viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int j = 0; j < image_height; j++) {
    std::clog << "\r Scanlines remaining: " << (image_height - j) << ' '
              << std::flush;
    for (int i = 0; i < image_width; i++) {
      point3 pixel_center =
          pixel00_location + (i * pixel_delta_u) + (j * pixel_delta_v);
      vec3 ray_dir = pixel_center - camera_center;
      ray ray(camera_center, ray_dir);

      color color = ray_color(ray, world);
      write_color(std::cout, color);
    }
  }
  std::clog << "\nDone.         \n";
}
