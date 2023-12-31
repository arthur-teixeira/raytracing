#include <iostream>

#include "camera.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "utils.h"
#include "vec3.h"

#define ASPECT_RATIO (16.0f / 9.0f)

int main() {
  hittable_list world;

  auto R = cos(pi / 4);

  auto mat_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
  auto mat_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
  auto mat_left = make_shared<dieletric>(1.5);
  auto mat_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

  world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, mat_ground));
  world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, mat_center));
  world.add(make_shared<sphere>(point3(-1, 0, -1), 0.5, mat_left));
  world.add(make_shared<sphere>(point3(-1, 0, -1), -0.4, mat_left));
  world.add(make_shared<sphere>(point3(1, 0, -1), 0.5, mat_right));

  camera cam = camera();
  cam.aspect_ratio = ASPECT_RATIO;
#ifdef PREVIEW
  cam.image_width = 480;
#else
  cam.image_width = 1920;
#endif // PREVIEW

  cam.vfov = 20;
  cam.lookfrom = point3(-2, 2, 1);
  cam.lookat = point3(0, 0, -1);
  cam.vup = vec3(0, 1, 0);
  cam.samples_per_pixel = 100;
  cam.defocus_angle = 10;
  cam.focus_dist = 3.4;
  cam.render(world);
}
