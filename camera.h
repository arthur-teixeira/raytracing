#ifndef CAMERA_H
#define CAMERA_H

#include "utils.h"

#include "color.h"
#include "hittable.h"

#define ASPECT_RATIO (16.0f / 9.0f)

class camera {
public:
  double aspect_ratio = 1;
  int image_width = 100;
  int samples_per_pixel = 10;

  void render(const hittable &world) {
    initialize();

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
      std::clog << "\r Scanlines remaining: " << (image_height - j) << ' ' << std::flush;
      for (int i = 0; i < image_width; i++) {
        color pixel_color(0, 0, 0);

        for (int sample = 0; sample < samples_per_pixel; sample++) {
          ray r = get_ray(i, j);
          pixel_color += ray_color(r, world);
        }

        write_color(std::cout, pixel_color, samples_per_pixel);
      }
    }
    std::clog << "\nDone.         \n";
  }

private:
  int image_height;
  point3 center;
  point3 pixel00_loc;
  vec3 pixel_delta_u;
  vec3 pixel_delta_v;

  void initialize() {
    image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;
    center = point3(0, 0, 0);

    double focal_length = 1.0f;
    double viewport_height = 2.0f;
    double viewport_width =
        viewport_height * (static_cast<double>(image_width) / image_height);

    vec3 viewport_u = vec3(viewport_width, 0, 0);
    vec3 viewport_v = vec3(0, -viewport_height, 0);

    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    point3 viewport_upper_left =
        center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;

    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
  }

  color ray_color(const ray &r, const hittable &world) const {
    hit_record rec;
    if (world.hit(r, interval(0, infinity), rec)) {
      return 0.5 * (rec.normal + color(1, 1, 1));
    }

    vec3 unit_direction = unit_vec(r.direction());
    double a = 0.5 * (unit_direction.y() + 1.0);

    return (1.0 - a) * color(1, 1, 1) + a * color(0.5, 0.7, 1.0);
  }

  ray get_ray(int i, int j) const {
    vec3 pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
    vec3 pixel_sample = pixel_center + pixel_sample_square();

    point3 ray_origin = center;
    vec3 ray_dir = pixel_sample - ray_origin;

    return ray(ray_origin, ray_dir);
  }

  vec3 pixel_sample_square() const {
    double px = -0.5 * random_double();
    double py = -0.5 * random_double();

    return (px * pixel_delta_u) + (py * pixel_delta_v);
  }
};

#endif // !CAMERA_H
