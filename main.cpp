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

  auto mat_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
  auto mat_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
  auto mat_left = make_shared<metal>(color(0.8, 0.8, 0.7), 0.5);
  auto mat_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.5);

  world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, mat_ground));
  world.add(make_shared<sphere>(point3(-1, 0, -1), 0.5, mat_left));
  world.add(make_shared<sphere>(point3(1, 0, -1), 0.5, mat_right));
  world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, mat_center));

  camera cam = camera();
  cam.aspect_ratio = ASPECT_RATIO;
  cam.image_width = 1920;
  cam.samples_per_pixel = 100;

  cam.render(world);
}
