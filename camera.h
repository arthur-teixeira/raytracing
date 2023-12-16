#ifndef CAMERA_H
#define CAMERA_H

#include "utils.h"

#include "color.h"
#include "hittable.h"
#include "material.h"
#include "vec3.h"

#define ASPECT_RATIO (16.0f / 9.0f)

class camera {
public:
  double aspect_ratio = 1;
  int image_width = 100;
  int samples_per_pixel = 10;
  int max_depth = 10;

  double vfov = 90;
  point3 lookfrom = point3(0, 0, -1);
  point3 lookat = point3(0, 0, 0);
  vec3 vup = vec3(0, 1, 0);

  double defocus_angle = 0;
  double focus_dist = 10;

  void render(const hittable &world) {
    initialize();

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
      std::clog << "\r Scanlines remaining: " << (image_height - j) << ' '
                << std::flush;
      for (int i = 0; i < image_width; i++) {
        color pixel_color(0, 0, 0);

        for (int sample = 0; sample < samples_per_pixel; sample++) {
          ray r = get_ray(i, j);
          pixel_color += ray_color(r, max_depth, world);
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
  vec3 u, v, w;
  vec3 defocus_disk_u;
  vec3 defocus_disk_v;

  void initialize() {
    image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;
    center = lookfrom;

    double theta = degrees_to_radians(vfov);
    double h = tan(theta / 2);
    double viewport_height = 2.0f * h * focus_dist;
    double viewport_width =
        viewport_height * (static_cast<double>(image_width) / image_height);

    w = unit_vec(lookfrom - lookat);
    u = unit_vec(cross(vup, w));
    v = cross(w, u);

    vec3 viewport_u = viewport_width * u;
    vec3 viewport_v = viewport_height * -v;

    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    point3 viewport_upper_left =
        center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;

    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    double defocus_radius =
        focus_dist * tan(degrees_to_radians(defocus_angle / 2));
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;
  }

  color ray_color(const ray &r, int depth, const hittable &world) const {
    hit_record rec;

    if (depth <= 0) {
      return color(0, 0, 0);
    }

    if (world.hit(r, interval(0.001, infinity), rec)) {
      ray scattered;
      color attenuation;
      if (rec.mat->scatter(r, rec, attenuation, scattered)) {
        return attenuation * ray_color(scattered, depth - 1, world);
      }

      return color(0, 0, 0);
    }

    vec3 unit_direction = unit_vec(r.direction());
    double a = 0.5 * (unit_direction.y() + 1.0);

    return (1.0 - a) * color(1, 1, 1) + a * color(0.5, 0.7, 1.0);
  }

  ray get_ray(int i, int j) const {
    vec3 pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
    vec3 pixel_sample = pixel_center + pixel_sample_square();

    point3 ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
    vec3 ray_dir = pixel_sample - ray_origin;

    return ray(ray_origin, ray_dir);
  }

  vec3 pixel_sample_square() const {
    double px = -0.5 * random_double();
    double py = -0.5 * random_double();

    return (px * pixel_delta_u) + (py * pixel_delta_v);
  }

  point3 defocus_disk_sample() const {
    vec3 p = random_in_unit_disk();
    return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
  }
};

#endif // !CAMERA_H
