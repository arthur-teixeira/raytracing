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
  world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));
  world.add(make_shared<sphere>(point3(0,0,-1), 0.5));

  camera cam = camera();
  cam.aspect_ratio = ASPECT_RATIO;
  cam.image_width = 480;
  cam.samples_per_pixel = 100;

  cam.render(world);
}
