#include <iostream>

#include "color.h"
#include "ray.h"
#include "vec3.h"

#define ASPECT_RATIO (16.0f / 9.0f)

double hit_sphere(const point3 &center, double radius, const ray &r) {
  vec3 oc = r.origin() - center;
  double a = r.direction().length_squared();
  double half_b = dot(oc, r.direction());
  double c = oc.length_squared() - radius * radius;
  double discriminant = half_b * half_b - a * c;

  if (discriminant < 0) {
    return -1.0;
  }

  return (-half_b - sqrt(discriminant)) / a;
}

color ray_color(const ray &r) {
  double t = hit_sphere(point3(0, 0, -1), 0.5, r);
  if (t > 0) {
    vec3 N = unit_vec(r.at(t) - vec3(0, 0, -1));
    return 0.5 * (N + 1);
  }

  vec3 unit_direction = unit_vec(r.direction());
  double a = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - a) * color(1, 1, 1) + a * color(0.5, 0.7, 1.0);
}

int main() {
  int image_width = 400;
  int image_height = static_cast<int>(image_width / ASPECT_RATIO);
  image_height = (image_height < 1) ? 1 : image_height;

  std::clog << "w = " << image_width << ", h = " << image_height << '\n';

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

      color color = ray_color(ray);
      write_color(std::cout, color);
    }
  }
  std::clog << "\nDone.         \n";
}
